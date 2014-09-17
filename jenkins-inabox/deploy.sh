stackato target https://api.${DOMAIN}
stackato login terryhowe
stackato unmap inabox inabox.${DOMAIN} || true
stackato stop inabox
stackato push -n
echo "Deploying http://inabox.${DOMAIN}"
stackato start inabox
stackato map inabox inabox.${DOMAIN}

