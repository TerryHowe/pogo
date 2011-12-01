#ifndef TestScanner_h
#define TestScanner_h

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


class TestScanner : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( TestScanner );
    CPPUNIT_TEST( testParse );
    CPPUNIT_TEST_SUITE_END();
public:
    TestScanner() { }
    void setUp(void) {}
    void tearDown(void) {}
    void testParse();
};

#endif
