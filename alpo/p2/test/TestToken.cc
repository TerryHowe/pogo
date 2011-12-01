#include "Token.h"
#include "TestToken.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestToken );

void
TestToken::testParse() {
    AToken token;

    CPPUNIT_ASSERT(token.parse("").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kNothing, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse("[").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kLeftBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse("]").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kRightBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse(":").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kColon, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse("[ asdf ]").compare(" asdf ]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kLeftBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse(" \r\n\t\n").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kNothing, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse(" \r\n\t\nasdf ]\n").compare("]\n") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("asdf") == 0);

    CPPUNIT_ASSERT(token.parse("asdf").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("asdf") == 0);

    CPPUNIT_ASSERT(token.parse("asdf asdf asdf").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("asdf asdf asdf") == 0);

    CPPUNIT_ASSERT(token.parse("asdf asdf asdf ").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("asdf asdf asdf") == 0);

    CPPUNIT_ASSERT(token.parse("[Section demonstration]").compare("Section demonstration]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kLeftBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse("Section demonstration]").compare("]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("Section demonstration") == 0);

    CPPUNIT_ASSERT(token.parse("[  Section demonstration]").compare("  Section demonstration]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kLeftBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse("  Section demonstration]").compare("]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("Section demonstration") == 0);

    CPPUNIT_ASSERT(token.parse("[ Section demonstration \t ]").compare(" Section demonstration \t ]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kLeftBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse(" Section demonstration \t ]").compare("]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("Section demonstration") == 0);

    CPPUNIT_ASSERT(token.parse("key: value").compare(": value") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("key") == 0);

    CPPUNIT_ASSERT(token.parse(": value").compare(" value") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kColon, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse(" value").compare("") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("value") == 0);

    CPPUNIT_ASSERT(token.parse("key : value").compare(": value") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("key") == 0);

    CPPUNIT_ASSERT(token.parse("key:value").compare(":value") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("key") == 0);

    CPPUNIT_ASSERT(token.parse(":value").compare("value") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kColon, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse(" [ asdf ]").compare(" asdf ]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kUnexpected, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    CPPUNIT_ASSERT(token.parse(" asdf [ fdsa ]").compare("[ fdsa ]") == 0);
    CPPUNIT_ASSERT_EQUAL(AToken::kUnexpected, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("asdf") == 0);
}
