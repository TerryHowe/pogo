#include "FixedPoint.h"
#include "TestFixedPoint.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestFixedPoint );

void
TestFixedPoint::testIt() {
    AFixedPoint fixie;
    fixie.setValue((float)1.0);
    CPPUNIT_ASSERT_EQUAL((unsigned)0x00008000, (unsigned)fixie.getValue());
    fixie.setValue((float)-1.0);
    CPPUNIT_ASSERT_EQUAL((unsigned)0x80008000, (unsigned)fixie.getValue());
    fixie.setValue((float)2.0);
    CPPUNIT_ASSERT_EQUAL((unsigned)0x00010000, (unsigned)fixie.getValue());
    fixie.setValue((float)-2.5);
    CPPUNIT_ASSERT_EQUAL((unsigned)0x80014000, (unsigned)fixie.getValue());
    fixie.setValue((float)3.14);
    CPPUNIT_ASSERT_EQUAL((unsigned)0x000191eb, (unsigned)fixie.getValue());
    fixie.setValue((float)100.99);
    CPPUNIT_ASSERT_EQUAL((unsigned)0x00327eb8, (unsigned)fixie.getValue());

    fixie.setValue((UInt32)0x00008000);
    CPPUNIT_ASSERT_EQUAL((float)1.0, fixie.getFloat());
    fixie.setValue((UInt32)0x80008000);
    CPPUNIT_ASSERT_EQUAL((float)-1.0, fixie.getFloat());
    fixie.setValue((UInt32)0x00010000);
    CPPUNIT_ASSERT_EQUAL((float)2.0, fixie.getFloat());
    fixie.setValue((UInt32)0x80014000);
    CPPUNIT_ASSERT_EQUAL((float)-2.5, fixie.getFloat());
    fixie.setValue((UInt32)0x000191eb);
    CPPUNIT_ASSERT_EQUAL((float)3.14, fixie.getFloat());
    fixie.setValue((UInt32)0x00327eb8);
    CPPUNIT_ASSERT_EQUAL((float)100.99, fixie.getFloat());
}
