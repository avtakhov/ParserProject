#ifndef PARSER_EXCEPTIONS_H_INCLUDED
#define PARSER_EXCEPTIONS_H_INCLUDED

#include <string>

struct ParamsChunk
{
    size_t begin;
    size_t end;
};

class ParsingException : public std::exception
{
public:
    ParsingException(const std::string& description, const std::string& params, const ParamsChunk& position);
    virtual ~ParsingException() = default;

    const char* what() const noexcept override;
    ParamsChunk GetErrorPosition() const;
    std::string GetErrorPart() const;

private:
    static std::string FormatFullDescription(const std::string& description, const std::string& params, const ParamsChunk& position);

private:
    const std::string m_description;
    const std::string m_params;
    const ParamsChunk m_errorPos;
};

class MissingParameterNameException : public ParsingException
{
public:
    MissingParameterNameException(const std::string& params, const ParamsChunk& position);
};

class UnexpectedValueException : public ParsingException
{
public:
    UnexpectedValueException(const std::string& params, const ParamsChunk& position);
};

class SpecifiedTwiceParameterException : public ParsingException
{
public:
    SpecifiedTwiceParameterException(const std::string& params, const ParamsChunk& position);
};

#endif // PARSER_EXCEPTIONS_H_INCLUDED

