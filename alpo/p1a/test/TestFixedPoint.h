#ifndef TestFixedPoint_h
#define TestFixedPoint_h

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


class TestFixedPoint : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( TestFixedPoint );
    CPPUNIT_TEST( testIt );
    CPPUNIT_TEST_SUITE_END();
public:
    TestFixedPoint() { }
    void setUp(void) { }
    void tearDown(void) { }
    void testIt();
};

#endif
