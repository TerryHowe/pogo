#ifndef TestConfigurationFile_h
#define TestConfigurationFile_h

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>


class TestConfigurationFile : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE( TestConfigurationFile );
    CPPUNIT_TEST( testRead );
    CPPUNIT_TEST( testWrite );
    CPPUNIT_TEST( testData );
    CPPUNIT_TEST( testErrors );
    CPPUNIT_TEST_SUITE_END();
public:
    TestConfigurationFile() { }
    void setUp(void) {
        (void)unlink("tmp.cfg");
    }
    void tearDown(void) {
        (void)unlink("tmp.cfg");
    }
    void testRead();
    void testWrite();
    void testData();
    void testErrors();
};

#endif
