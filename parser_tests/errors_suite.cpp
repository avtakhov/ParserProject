#include <gtest/gtest.h>
#include <params_parser/params_parser.h>
#include <params_parser/parser_exceptions.h>

using namespace std;

namespace
{
    const auto MISSING_PARAM_NAME_INPUT = "/first 1 / 2"s;
    const auto UNEXPECTED_VALUE_INPUT = "/verbosity quiet something else"s;
    const auto SPECIFIED_TWICE_PARAM_INPUT = "/verbosity debug /verbosity quiet"s;
}

TEST(ErrorsSuite, MissingParamNameTest)
{
    ASSERT_THROW(ParseParams(MISSING_PARAM_NAME_INPUT), MissingParameterNameException);
}

TEST(ErrorsSuite, MissingParamNameDetailsTest)
{
    try
    {
        ParseParams(MISSING_PARAM_NAME_INPUT);
    }
    catch(const MissingParameterNameException& ex)
    {
        const auto pos = ex.GetErrorPosition();
        ASSERT_EQ(9, pos.begin);
        ASSERT_EQ(10, pos.end);
        ASSERT_EQ("/", ex.GetErrorPart());
        ASSERT_EQ("Parameter name is missing at [10, 10] in \"" + MISSING_PARAM_NAME_INPUT + "\"", ex.what());
    }
}

TEST(ErrorsSuite, UnexpectedValueTest)
{
    ASSERT_THROW(ParseParams(UNEXPECTED_VALUE_INPUT), UnexpectedValueException);
}

TEST(ErrorsSuite, UnexpectedValueDetailsTest)
{
    try
    {
        ParseParams(UNEXPECTED_VALUE_INPUT);
    }
    catch(const UnexpectedValueException& ex)
    {
        const auto pos = ex.GetErrorPosition();
        ASSERT_EQ(17, pos.begin);
        ASSERT_EQ(26, pos.end);
        ASSERT_EQ("something", ex.GetErrorPart());
        ASSERT_EQ("Unexpected value at [18, 26] in \"" + UNEXPECTED_VALUE_INPUT + "\"", ex.what());
    }
}

TEST(ErrorsSuite, SpecifiedTwiceParamTest)
{
    ASSERT_THROW(ParseParams(SPECIFIED_TWICE_PARAM_INPUT), SpecifiedTwiceParameterException);
}

TEST(ErrorsSuite, SpecifiedTwiceParamNameTest)
{
    try
    {
        ParseParams(SPECIFIED_TWICE_PARAM_INPUT);
    }
    catch(const SpecifiedTwiceParameterException& ex)
    {
        const auto pos = ex.GetErrorPosition();
        ASSERT_EQ(17, pos.begin);
        ASSERT_EQ(27, pos.end);
        ASSERT_EQ("/verbosity", ex.GetErrorPart());
        ASSERT_EQ("Parameter is specified twice at [18, 27] in \"" + SPECIFIED_TWICE_PARAM_INPUT + "\"", ex.what());
    }
}

