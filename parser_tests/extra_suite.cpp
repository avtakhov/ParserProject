#include <gtest/gtest.h>
#include <params_parser/params_parser.h>
#include <iomanip>

using namespace std;

TEST(ExtraSuite, EmptyValueInQuotes)
{
    const map<string, string> EXPECTED = {
        { "param", "" }
    };
    const auto result = ParseParams("/param \"\"");
    ASSERT_EQ(EXPECTED, result);
}

TEST(ExtraSuite, OneLetterInQuotes)
{
    const map<string, string> EXPECTED = {
        { "param", "x" }
    };
    const auto result = ParseParams("/param \"x\"");
    ASSERT_EQ(EXPECTED, result);
}

TEST(ExtraSuite, OneSpaceInQuotes)
{
    const map<string, string> EXPECTED = {
        { "param", " " }
    };
    const auto result = ParseParams("/param \" \"");
    ASSERT_EQ(EXPECTED, result);
}

TEST(ExtraSuite, EscapedBackslashInQuotes)
{
    const map<string, string> EXPECTED = {
        { "param", "\\" },
    };
    const auto result = ParseParams(R"(/param "\\")");
    ASSERT_EQ(EXPECTED, result);
}

TEST(ExtraSuite, QuotedValueWithQuote)
{
    const map<string, string> EXPECTED = {
        { "doubleQuote", "\"" },
        { "singleQuote", "\'" }
    };
    const auto result = ParseParams("/doubleQuote \"\\\"\" /singleQuote \"\\\'\"");
    ASSERT_EQ(EXPECTED, result);
}

TEST(ExtraSuite, QuotedValueWithSlashAndQuote)
{
    const map<string, string> EXPECTED = {
        { "slashAndDoubleQ", R"(/")" },
        { "slashAndSingleQ", R"(/')" }
    };
    const auto result = ParseParams(R"(/slashAndDoubleQ "/\"" /slashAndSingleQ "/\'")");
    ASSERT_EQ(EXPECTED, result);
}

TEST(ExtraSuite, QuotedValueWithBackSlashAndQuote)
{
    const map<string, string> EXPECTED = {
        { "backslashAndDoubleQ", R"(\")" },
        { "backslashAndSingleQ", R"(\')" }
    };
    const auto result = ParseParams(R"(/backslashAndDoubleQ "\\\"" /backslashAndSingleQ "\\\'")");
    ASSERT_EQ(EXPECTED, result);
}

TEST(ExtraSuite, ValueWithEscapedParams)
{
    const auto SUBSTITUTION_MARK = "%RECURSION%"s;

    auto Substitute = [&SUBSTITUTION_MARK](const string& external, const string& internal)
    {
        const auto markPos = external.find(SUBSTITUTION_MARK);
        stringstream ss;
        ss
            << external.substr(0, markPos)
            << quoted(internal)
            << external.substr(markPos + SUBSTITUTION_MARK.length())
            ;
        return ss.str();
    };

    const auto PATTERN = "/a v /b \"v1 v2\" /r " + SUBSTITUTION_MARK + " /c";
    map<string, string> EXPECTED = {
        {"a", "v"},
        {"b", "v1 v2"},
        {"r", SUBSTITUTION_MARK},
        {"c", ""}
    };
    auto params = "end"s;
    for(int i = 0; i < 10; ++i)
    {
        EXPECTED["r"] = params;
        params = Substitute(PATTERN, params);

        ASSERT_EQ(EXPECTED, ParseParams(params));
    }
}

