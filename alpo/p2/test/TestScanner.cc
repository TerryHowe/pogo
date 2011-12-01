#include "Scanner.h"
#include "TestScanner.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestScanner );

void
TestScanner::testParse() {
    AToken token;
    AScanner scanner;

    scanner.getLine() = "[kdkdkd]\nkey:value\n";

    token = scanner.getNextToken();
    CPPUNIT_ASSERT_EQUAL(AToken::kLeftBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    token = scanner.getNextToken();
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("kdkdkd") == 0);

    token = scanner.getNextToken();
    CPPUNIT_ASSERT_EQUAL(AToken::kRightBracket, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    token = scanner.getNextToken();
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("key") == 0);

    token = scanner.getNextToken();
    CPPUNIT_ASSERT_EQUAL(AToken::kColon, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);

    token = scanner.getNextToken();
    CPPUNIT_ASSERT_EQUAL(AToken::kString, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("value") == 0);

    token = scanner.getNextToken();
    CPPUNIT_ASSERT_EQUAL(AToken::kNothing, token.getType());
    CPPUNIT_ASSERT(token.getValue().compare("") == 0);
}
