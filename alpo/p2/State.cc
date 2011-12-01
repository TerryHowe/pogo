#include <iostream>

#include "State.h"

using namespace std;

const AStateSectionStart AStateSectionStart::instance;
const AStateSectionName AStateSectionName::instance;
const AStateSectionEnd AStateSectionEnd::instance;
const AStateKeyName AStateKeyName::instance;
const AStateKeyColon AStateKeyColon::instance;
const AStateKeyValue AStateKeyValue::instance;

const AState*
AStateSectionStart::process(AConfigurationFile& context, const AToken& token)
    const
{
    if (token.getType() == AToken::kLeftBracket)
        return &AStateSectionName::instance;
    cerr << "ERROR: Expected start of section '['" << endl;
    return &AStateSectionStart::instance;
}

const AState*
AStateSectionName::process(AConfigurationFile& context, const AToken& token)
    const
{
    if (token.getType() == AToken::kString)
    {
        context.setSection(token.getValue());
        return &AStateSectionEnd::instance;
    }
    cerr << "ERROR: Expected section name" << endl;
    if (token.getType() == AToken::kLeftBracket)
        return &AStateSectionName::instance;
    return &AStateSectionStart::instance;
}

const AState*
AStateSectionEnd::process(AConfigurationFile& context, const AToken& token)
    const
{
    if (token.getType() == AToken::kRightBracket)
        return &AStateKeyName::instance;
    cerr << "ERROR: Expected end of section ']'" << endl;
    if (token.getType() == AToken::kLeftBracket)
        return &AStateSectionName::instance;
    return &AStateSectionStart::instance;
}

const AState*
AStateKeyName::process(AConfigurationFile& context, const AToken& token)
    const
{
    if (token.getType() == AToken::kString)
    {
        context.setKeyName(token.getValue());
        return &AStateKeyColon::instance;
    }
    if (token.getType() == AToken::kLeftBracket)
        return &AStateSectionName::instance;
    cerr << "ERROR: Expected end of section ']'" << endl;
    return &AStateKeyName::instance;
}

const AState*
AStateKeyColon::process(AConfigurationFile& context, const AToken& token)
    const
{
    if (token.getType() == AToken::kColon)
        return &AStateKeyValue::instance;
    cerr << "ERROR: Expected ':' of key value pair" << endl;
    if (token.getType() == AToken::kLeftBracket)
        return &AStateSectionName::instance;
    return &AStateKeyName::instance;
}

const AState*
AStateKeyValue::process(AConfigurationFile& context, const AToken& token)
    const
{
    if (token.getType() == AToken::kString)
    {
        context.setKeyValue(token.getValue());
        return &AStateKeyName::instance;
    }
    cerr << "ERROR: Expected key value" << endl;
    if (token.getType() == AToken::kLeftBracket)
        return &AStateSectionName::instance;
    return &AStateKeyName::instance;
}
