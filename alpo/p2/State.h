#ifndef h_State
#define h_State

#include <string>

#include "AlTypes.h"
#include "Token.h"
#include "ConfigurationFile.h"

//
// AState is the base state class for parsing the configuration file
//
class AState
{
private:
    std::string fName;
protected:
    AState(std::string name) { this->fName = name; }
public:
    ~AState() {}
    std::string name() const { return this->fName; }
    virtual const AState* process(AConfigurationFile& context,
                                  const AToken& token) const { return(this); }
};

//
// AStateSectionStart looks for the left bracket
//
class AStateSectionStart : public AState
{
public:
    static const AStateSectionStart instance;
    AStateSectionStart() : AState("SectionStart") { }
    virtual const AState* process(AConfigurationFile& context,
                                  const AToken& token) const;
};

//
// AStateSectionName looks for the section name
//
class AStateSectionName : public AState
{
public:
    static const AStateSectionName instance;
    AStateSectionName() : AState("SectionName") { }
    virtual const AState* process(AConfigurationFile& context,
                                  const AToken& token) const;
};

//
// AStateSectionEnd looks for the section end which is a ]
//
class AStateSectionEnd : public AState
{
public:
    static const AStateSectionEnd instance;
    AStateSectionEnd() : AState("SectionEnd") { }
    virtual const AState* process(AConfigurationFile& context,
                                  const AToken& token) const;
};

//
// AStateKeyName looks for the key name, a string
//
class AStateKeyName : public AState
{
public:
    static const AStateKeyName instance;
    AStateKeyName() : AState("KeyName") { }
    virtual const AState* process(AConfigurationFile& context,
                                  const AToken& token) const;
};

//
// AStateKeyColon looks for a colon
//
class AStateKeyColon : public AState
{
public:
    static const AStateKeyColon instance;
    AStateKeyColon() : AState("KeyColon") { }
    virtual const AState* process(AConfigurationFile& context,
                                  const AToken& token) const;
};

//
// AStateKeyValue looks for a string that is the value of a key
//
class AStateKeyValue : public AState
{
public:
    static const AStateKeyValue instance;
    AStateKeyValue() : AState("KeyValue") { }
    virtual const AState* process(AConfigurationFile& context,
                                  const AToken& token) const;
};

#endif
