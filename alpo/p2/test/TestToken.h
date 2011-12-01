#ifndef TestToken_h
#define TestToken_h

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


class TestToken : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( TestToken );
    CPPUNIT_TEST( testParse );
    CPPUNIT_TEST_SUITE_END();
public:
    TestToken() { }
    void setUp(void) {}
    void tearDown(void) {}
    void testParse();
};

#endif
