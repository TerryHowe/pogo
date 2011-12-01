#include "State.h"
#include "TestState.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TestState );

void
TestState::testSectionStart() {
    AStateSectionStart state;

    tok.setType(AToken::kLeftBracket);
    CPPUNIT_ASSERT(state.process(con,tok)->name().compare("SectionName") == 0);
    tok.setType(AToken::kRightBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionStart") == 0);
    tok.setType(AToken::kColon);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionStart") == 0);
    tok.setType(AToken::kString);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionStart") == 0);
}

void
TestState::testSectionName() {
    AStateSectionName state;

    tok.setType(AToken::kLeftBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionName") == 0);
    tok.setType(AToken::kRightBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionStart") == 0);
    tok.setType(AToken::kColon);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionStart") == 0);
    CPPUNIT_ASSERT(con.getSection().compare("") == 0);
    tok.setType(AToken::kString);
    tok.setValue("asdf");
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionEnd") == 0);
    CPPUNIT_ASSERT(con.getSection().compare("asdf") == 0);
}

void
TestState::testSectionEnd() {
    AStateSectionEnd state;

    tok.setType(AToken::kLeftBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionName") == 0);
    tok.setType(AToken::kRightBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
    tok.setType(AToken::kColon);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionStart") == 0);
    tok.setType(AToken::kString);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionStart") == 0);
}

void
TestState::testKeyName() {
    AStateKeyName state;

    tok.setType(AToken::kLeftBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionName") == 0);
    tok.setType(AToken::kRightBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
    tok.setType(AToken::kColon);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
    CPPUNIT_ASSERT(con.getKeyName().compare("") == 0);
    tok.setType(AToken::kString);
    tok.setValue("keyName");
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyColon") == 0);
    CPPUNIT_ASSERT(con.getKeyName().compare("keyName") == 0);
}

void
TestState::testKeyColon() {
    AStateKeyColon state;

    tok.setType(AToken::kLeftBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionName") == 0);
    tok.setType(AToken::kRightBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
    tok.setType(AToken::kColon);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyValue") == 0);
    tok.setType(AToken::kString);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
}

void
TestState::testKeyValue() {
    AStateKeyValue state;

    tok.setType(AToken::kLeftBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("SectionName") == 0);
    tok.setType(AToken::kRightBracket);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
    tok.setType(AToken::kColon);
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
    con.setSection("section");
    con.setKeyName("keyName");
    tok.setType(AToken::kString);
    tok.setValue("keyValue");
    CPPUNIT_ASSERT(state.process(con, tok)->name().compare("KeyName") == 0);
    CPPUNIT_ASSERT(con.get("section", "keyName").compare("keyValue") == 0);
}
