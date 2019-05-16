#include <gtest/gtest.h>
#include <params_parser/params_parser.h>

using namespace std;

TEST(BasicSuite, EmptyTest)
{
    const auto result = ParseParams(""s);
    ASSERT_TRUE(result.empty());
}

TEST(BasicSuite, OneParamTest)
{
    const map<string, string> EXPECTED = {
        { "param", "value" }
    };
    const auto result = ParseParams("/param value");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, OneLetterValueTest)
{
    const map<string, string> EXPECTED = {
        { "param", "x" }
    };
    const auto result = ParseParams("/param x");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, TwoParamsTest)
{
    const map<string, string> EXPECTED = {
        { "first_param", "first_value" },
        { "second_param", "second_value" }
    };
    const auto result = ParseParams("/first_param first_value /second_param second_value");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, ParamWithoutValue)
{
    const map<string, string> EXPECTED = {
        { "silent", "" }
    };
    const auto result = ParseParams("/silent");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, TwoParamsWithoutValue)
{
    const map<string, string> EXPECTED = {
        { "silent", "" },
        { "reboot", "" }
    };
    const auto result = ParseParams("/silent /reboot");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, ValueWithSpace)
{
    const map<string, string> EXPECTED = {
        { "name", "John Doe" }
    };
    const auto result = ParseParams("/name \"John Doe\"");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, TwoValuesWithSpaces)
{
    const map<string, string> EXPECTED = {
        { "name", "Jane Doe" },
        { "city", "Default City" }
    };
    const auto result = ParseParams("/name \"Jane Doe\" /city \"Default City\"");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, ValueWithSlashes)
{
    const map<string, string> EXPECTED = {
        { "unix_path", "/home/username/Desktop" },
        { "windowsPath", "C:\\Users\\username\\Desktop" }
    };
    const auto result = ParseParams("/unix_path \"/home/username/Desktop\" /windowsPath C:\\Users\\username\\Desktop");
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, MixedParamTypes)
{
    const map<string, string> EXPECTED = {
        {"silent", ""}, {"verbosity", "quiet"}, {"installdir", "C:\\Program Files\\My Application"},
        {"reboot", ""}, {"email", "username@example.com"}, {"licenseKey", "C:\\Users\\username\\Desktop\\license.key"}
    };
    const auto result = ParseParams(
        "/silent "
        "/verbosity quiet "
        "/installdir \"C:\\Program Files\\My Application\" "
        "/reboot "
        "/email \"username@example.com\" "
        "/licenseKey \"C:\\Users\\username\\Desktop\\license.key\""
    );
    ASSERT_EQ(EXPECTED, result);
}

TEST(BasicSuite, MixedParamTypesWithTabs)
{
    const map<string, string> EXPECTED = {
        {"silent", ""}, {"verbosity", "quiet"}, {"installdir", "C:\\Program Files\\My Application"},
        {"reboot", ""}, {"email", "username@example.com"}, {"licenseKey", "C:\\Users\\username\\Desktop\\license.key"},
        {"text", "Some text\twith\ttabs"}
    };
    const auto result = ParseParams(
        "/silent\t"
        "/verbosity quiet \t"
        "/installdir \"C:\\Program Files\\My Application\" \t "
        "/reboot    "
        "/email \t \"username@example.com\" "
        "/licenseKey\t\t\t\"C:\\Users\\username\\Desktop\\license.key\"\t\t\t"
        "/text \"Some text\twith\ttabs\""
    );
    ASSERT_EQ(EXPECTED, result);
}
