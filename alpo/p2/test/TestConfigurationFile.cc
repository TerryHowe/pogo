#include "ConfigurationFile.h"
#include "TestConfigurationFile.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestConfigurationFile );

static const char* testCfg = "test.cfg";
static const char* tmpCfg = "/tmp/tmp.cfg";
static const char* dataCfg = "data.cfg";
static const char* errorCfg = "error.cfg";

void
TestConfigurationFile::testRead()
{
    AConfigurationFile file("test.cfg");
    CPPUNIT_ASSERT(file.read());
}

void
TestConfigurationFile::testWrite()
{
    (void)unlink(tmpCfg);
    AConfigurationFile missing(tmpCfg);
    CPPUNIT_ASSERT(missing.read() == false);
    CPPUNIT_ASSERT(missing.write());

    AConfigurationFile reader(tmpCfg);
    CPPUNIT_ASSERT(reader.read());
    CPPUNIT_ASSERT(reader.get("s1", "s1k1").compare("") == 0);
    CPPUNIT_ASSERT(reader.get("s1", "s1k2").compare("") == 0);
    reader.setSection("s1");
    reader.setKeyName("s1k1");
    reader.setKeyValue(1.1);
    CPPUNIT_ASSERT(reader.write());

    AConfigurationFile gotOne(tmpCfg);
    CPPUNIT_ASSERT(gotOne.read());
    CPPUNIT_ASSERT(gotOne.get("s1", "s1k1").compare("1.1") == 0);
    CPPUNIT_ASSERT(gotOne.get("s1", "s1k2").compare("") == 0);
    gotOne.setKeyName("s1k2");
    gotOne.setKeyValue("1.2");

    gotOne.setSection("s2");
    gotOne.setKeyName("s2k1");
    gotOne.setKeyValue("2.1");
    gotOne.setKeyName("s2k2");
    gotOne.setKeyValue("2.2");

    gotOne.setSection("s3");
    gotOne.setKeyName("s3k1");
    gotOne.setKeyValue(3);
    CPPUNIT_ASSERT(gotOne.write());

    AConfigurationFile gotLots(tmpCfg);
    CPPUNIT_ASSERT(gotLots.read());
    CPPUNIT_ASSERT(gotLots.get("s1", "s1k1").compare("1.1") == 0);
    CPPUNIT_ASSERT(gotLots.get("s1", "s1k2").compare("1.2") == 0);
    CPPUNIT_ASSERT(gotLots.get("s2", "s2k1").compare("2.1") == 0);
    CPPUNIT_ASSERT(gotLots.get("s2", "s2k2").compare("2.2") == 0);
    CPPUNIT_ASSERT_EQUAL((float)2.2, gotLots.getFloat("s2", "s2k2"));
    CPPUNIT_ASSERT(gotLots.get("s3", "s3k1").compare("3") == 0);
    CPPUNIT_ASSERT_EQUAL(3, gotLots.getInt("s3", "s3k1"));

    (void)unlink(tmpCfg);
}

void
TestConfigurationFile::testData()
{
    AConfigurationFile data(dataCfg);
    CPPUNIT_ASSERT(data.read());
    CPPUNIT_ASSERT(data.get("header", "project").compare("Programming Test") == 0);
    CPPUNIT_ASSERT(data.get("header", "budget").compare("4.5") == 0);
    CPPUNIT_ASSERT_EQUAL((float)4.5, data.getFloat("header", "budget"));
    CPPUNIT_ASSERT(data.get("header", "accessed").compare("205") == 0);
    CPPUNIT_ASSERT_EQUAL(205, data.getInt("header", "accessed"));
    CPPUNIT_ASSERT(data.get("meta data", "description").compare("This is a tediously long description of the Art & Logic programming test that you are taking. Tedious isn't the right word, but it's the first word that comes to mind.") == 0);
    CPPUNIT_ASSERT(data.get("meta data", "correction text").compare("I meant 'moderately,' not 'tediously,' above.") == 0);
    CPPUNIT_ASSERT(data.get("trailer", "budget").compare("all out of budget.") == 0);
}

void
TestConfigurationFile::testErrors()
{
    // Must manually check error messages
    cerr << "------------------Start section and key name errors:" << endl;
    AConfigurationFile data(errorCfg);
    CPPUNIT_ASSERT(data.read());
    cerr << "------------------End section and key name errors" << endl;
}
