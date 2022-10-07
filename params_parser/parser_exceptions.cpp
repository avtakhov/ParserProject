#include "parser_exceptions.h"

using namespace std;

ParsingException::ParsingException(const string& description, std::string_view params, const ParamsChunk& position) :
    m_description(FormatFullDescription(description, params, position)),
    m_params(params),
    m_errorPos(position)
{
}

ParamsChunk ParsingException::GetErrorPosition() const
{
    return m_errorPos;
}

string ParsingException::GetErrorPart() const
{
    return m_params.substr(m_errorPos.begin, m_errorPos.end - m_errorPos.begin);
}

string ParsingException::FormatFullDescription(const string& description, std::string_view params,
                                               const ParamsChunk& position)
{
    const auto positionStr = "[" + to_string(1 + position.begin) + ", " + to_string(position.end) + "]";
    return description + " at " + positionStr + " in \"" + string(params) + "\"";
}

MissingParameterNameException::MissingParameterNameException(std::string_view params, const ParamsChunk& position) :
    ParsingException("Parameter name is missing", params, position)
{
}

UnexpectedValueException::UnexpectedValueException(std::string_view params, const ParamsChunk& position) :
    ParsingException("Unexpected value", params, position)
{
}

SpecifiedTwiceParameterException::SpecifiedTwiceParameterException(std::string_view params, const ParamsChunk& position) :
    ParsingException("Parameter is specified twice", params, position)
{
}

MissingQuotesException::MissingQuotesException(std::string_view params, const ParamsChunk& position) :
    ParsingException("Missing terminating quotes character", params, position)
{
}

const char* ParsingException::what() const noexcept
{
    return m_description.c_str();
}

