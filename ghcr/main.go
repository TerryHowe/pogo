package main

import (
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"os"
	"regexp"
	"strings"
	"time"
)

// This tool performs a Docker Registry v2 Bearer Token login against ghcr.io.
// It requests the auth challenge from https://ghcr.io/v2/ and exchanges it for a token.
//
// Usage examples:
//   go run . -u USERNAME -p GITHUB_TOKEN -scope "repository:owner/image:pull"
//   GHCR_USER=foo GHCR_TOKEN=ghp_xxx go run . -repo owner/image -perm pull
//   go run .                            # anonymous token (limited) if allowed
//
// Note: For GitHub Container Registry (ghcr.io), the username can be anything ("USERNAME"),
// but typically your GitHub username; the password must be a GitHub Personal Access Token
// (classic) with the appropriate packages permissions.

type authChallenge struct {
	Realm   string
	Service string
}

type tokenResponse struct {
	Token       string `json:"token"`
	AccessToken string `json:"access_token"`
	ExpiresIn   int    `json:"expires_in"`
	IssuedAt    string `json:"issued_at"`
}

func main() {
	var (
		username = flag.String("u", envOr("GHCR_USER", ""), "Username (or set GHCR_USER)")
		password = flag.String("p", envOr("GHCR_TOKEN", ""), "Password/Token (or set GHCR_TOKEN)")
		scope    = flag.String("scope", "", "Scope like 'repository:owner/image:pull' (overrides -repo/-perm)")
		repo     = flag.String("repo", "", "Repository in form 'owner/image' to build scope")
		perm     = flag.String("perm", "pull", "Permission for scope when -repo is set (pull|push|*). Default: pull")
		verify   = flag.Bool("verify", true, "Verify token by listing tags when -repo is set")
		debug    = flag.Bool("debug", false, "Enable debug logging")
	)
	flag.Parse()

	client := &http.Client{Timeout: 15 * time.Second}

	ch, err := fetchAuthChallenge(client, *debug)
	if err != nil {
		fatalErr("fetch auth challenge", err)
	}

	resolvedScope := strings.TrimSpace(*scope)
	if resolvedScope == "" && *repo != "" {
		resolvedScope = fmt.Sprintf("repository:%s:%s", *repo, *perm)
	}

	tok, expiresIn, err := requestBearerToken(client, ch, *username, *password, resolvedScope, *debug)
	if err != nil {
		fatalErr("request bearer token", err)
	}

	fmt.Println("Login successful to ghcr.io using bearer authentication.")
	fmt.Printf("Token: %s\n", tok)
	if expiresIn > 0 {
		fmt.Printf("Expires in: %ds\n", expiresIn)
	}

	if *verify && *repo != "" {
		if err := verifyToken(client, tok, *repo, *debug); err != nil {
			fatalErr("verify token", err)
		}
		fmt.Println("Verification succeeded: able to access repository with provided token.")
	}
}

func envOr(k, def string) string {
	if v := os.Getenv(k); v != "" {
		return v
	}
	return def
}

func fetchAuthChallenge(client *http.Client, debug bool) (authChallenge, error) {
	u := "https://ghcr.io/v2/"
	req, err := http.NewRequest(http.MethodGet, u, nil)
	if err != nil {
		return authChallenge{}, err
	}
	resp, err := client.Do(req)
	if err != nil {
		return authChallenge{}, err
	}
	defer resp.Body.Close()
	io.Copy(io.Discard, resp.Body)

	if debug {
		fmt.Printf("DEBUG: GET %s -> %d\n", u, resp.StatusCode)
	}

	// Expect 401 with WWW-Authenticate: Bearer realm=...,service=...
	h := resp.Header.Get("Www-Authenticate")
	if h == "" {
		return authChallenge{}, fmt.Errorf("missing WWW-Authenticate header; status=%d", resp.StatusCode)
	}

	if debug {
		fmt.Printf("DEBUG: WWW-Authenticate: %s\n", h)
	}

	// Parse realm and service
	ch := authChallenge{}
	// regex tolerant to quotes and order
	re := regexp.MustCompile(`(?i)Bearer\s+(.*)`) // capture params
	m := re.FindStringSubmatch(h)
	paramStr := ""
	if len(m) == 2 {
		paramStr = m[1]
	} else {
		paramStr = h
	}
	for _, part := range splitAuthParams(paramStr) {
		kv := strings.SplitN(part, "=", 2)
		if len(kv) != 2 {
			continue
		}
		k := strings.ToLower(strings.TrimSpace(kv[0]))
		v := strings.Trim(strings.TrimSpace(kv[1]), "\"")
		switch k {
		case "realm":
			ch.Realm = v
		case "service":
			ch.Service = v
		}
	}

	if ch.Realm == "" {
		// Fallback default known for ghcr
		ch.Realm = "https://ghcr.io/token"
	}
	if ch.Service == "" {
		ch.Service = "ghcr.io"
	}

	return ch, nil
}

func splitAuthParams(s string) []string {
	// split on commas not inside quotes (simple parser)
	var parts []string
	var cur strings.Builder
	inQuote := false
	for i := 0; i < len(s); i++ {
		c := s[i]
		if c == '"' {
			inQuote = !inQuote
			cur.WriteByte(c)
			continue
		}
		if c == ',' && !inQuote {
			p := strings.TrimSpace(cur.String())
			if p != "" {
				parts = append(parts, p)
			}
			cur.Reset()
			continue
		}
		cur.WriteByte(c)
	}
	p := strings.TrimSpace(cur.String())
	if p != "" {
		parts = append(parts, p)
	}
	return parts
}

func requestBearerToken(client *http.Client, ch authChallenge, username, password, scope string, debug bool) (string, int, error) {
	q := url.Values{}
	if ch.Service != "" {
		q.Set("service", ch.Service)
	} else {
		q.Set("service", "ghcr.io")
	}
	if scope != "" {
		q.Set("scope", scope)
	}
	tokenURL := ch.Realm
	if tokenURL == "" {
		tokenURL = "https://ghcr.io/token"
	}
	fullURL := tokenURL + "?" + q.Encode()

	if debug {
		fmt.Printf("DEBUG: Token URL: %s\n", fullURL)
	}

	req, err := http.NewRequest(http.MethodGet, fullURL, nil)
	if err != nil {
		return "", 0, err
	}
	if username != "" || password != "" {
		req.SetBasicAuth(username, password)
	}
	resp, err := client.Do(req)
	if err != nil {
		return "", 0, err
	}
	defer resp.Body.Close()
	body, _ := io.ReadAll(resp.Body)
	if debug {
		fmt.Printf("DEBUG: Token resp status=%d body=%s\n", resp.StatusCode, truncate(string(body), 300))
	}
	if resp.StatusCode/100 != 2 {
		return "", 0, fmt.Errorf("token request failed: %s", resp.Status)
	}
	var tr tokenResponse
	if err := json.Unmarshal(body, &tr); err != nil {
		return "", 0, err
	}
	tok := tr.Token
	if tok == "" {
		tok = tr.AccessToken
	}
	if tok == "" {
		return "", 0, errors.New("no token in response")
	}
	return tok, tr.ExpiresIn, nil
}

func verifyToken(client *http.Client, token, repo string, debug bool) error {
	// Request small list to verify access
	u := fmt.Sprintf("https://ghcr.io/v2/%s/tags/list?n=1", repo)
	req, err := http.NewRequest(http.MethodGet, u, nil)
	if err != nil {
		return err
	}
	req.Header.Set("Authorization", "Bearer "+token)
	resp, err := client.Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()
	b, _ := io.ReadAll(resp.Body)
	if debug {
		fmt.Printf("DEBUG: Verify GET %s -> %d body=%s\n", u, resp.StatusCode, truncate(string(b), 300))
	}
	if resp.StatusCode/100 != 2 {
		return fmt.Errorf("verification failed: %s", resp.Status)
	}
	return nil
}

func truncate(s string, n int) string {
	if len(s) <= n {
		return s
	}
	return s[:n] + "..."
}

func fatalErr(phase string, err error) {
	fmt.Fprintf(os.Stderr, "Error during %s: %v\n", phase, err)
	os.Exit(1)
}
