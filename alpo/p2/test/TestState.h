#ifndef TestState_h
#define TestState_h

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Token.h"
#include "ConfigurationFile.h"


class TestState : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( TestState );
    CPPUNIT_TEST( testSectionStart );
    CPPUNIT_TEST( testSectionName );
    CPPUNIT_TEST( testSectionEnd );
    CPPUNIT_TEST( testKeyName );
    CPPUNIT_TEST( testKeyColon );
    CPPUNIT_TEST( testKeyValue );
    CPPUNIT_TEST_SUITE_END();

    AToken tok;
    AConfigurationFile con;
public:
    TestState() : con("empty.cfg") { }
    void setUp(void) {
        tok.setValue("");
    }
    void tearDown(void) {}
    void testSectionStart();
    void testSectionName();
    void testSectionEnd();
    void testKeyName();
    void testKeyColon();
    void testKeyValue();
};

#endif
