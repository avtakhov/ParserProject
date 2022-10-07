#include "params_parser.h"
#include "parser_exceptions.h"
#include "token.h"

#include <functional>

namespace parse
{

struct SingleParameter
{
    std::string_view key;
    std::string value;
};

void SkipWhitespaces(Source &source)
{
    if (source.GetCurrent().token == Token::Whitespaces) {
        source.Next();
    }
}

std::string_view Key(Source &source)
{
    const auto slash = source.ExpectOneOf({Token::Slash});
    const auto key = source.GetCurrent();
    if (key.token != Token::Word) {
        throw MissingParameterNameException(
            source.GetParams(),
            source.GetBounds(slash.value)
        );
    }
    source.Next();
    return key.value;
}

std::string ConcatMany(Source &source, const std::unordered_set<Token> &allowed)
{
    std::string result;
    while (source.CheckOneOf(allowed)) {
        result.append(source.GetCurrent().ToString());
        source.Next();
    }
    return result;
}

std::string UnquotedText(Source &source)
{
    const auto view = source.GetCurrent();
    std::string result;
    switch (view.token) {
        case Token::EscapedSequence:
            [[fallthrough]];
        case Token::Word:
            return ConcatMany(source, {
                Token::Word,
                Token::EscapedSequence,
                Token::Slash,
            });
        case Token::End:
            [[fallthrough]];
        case Token::Slash:
            return "";
        default:
            throw std::exception();
    }
}

std::string QuotedText(Source &source)
{
    auto open_quote = source.GetCurrent();
    source.Next();

    std::string result = ConcatMany(source, {
        Token::Slash,
        Token::Word,
        Token::Whitespaces,
        Token::Whitespaces,
        Token::EscapedSequence,
    });

    switch (source.GetCurrent().token) {
        case Token::Quote:
            source.Next();
            return result;
        default:
            throw MissingQuotesException(source.GetParams(), source.ToEnd(open_quote.value));
    }
}

std::string Value(Source &source)
{
    switch (source.GetCurrent().token) {
        case Token::Quote:
            return QuotedText(source);
        default:
            return UnquotedText(source);
    }
}

SingleParameter Param(Source &source)
{
    const auto key = Key(source);

    switch (source.GetCurrent().token) {
        case Token::End:
            return SingleParameter{key, ""};
        case Token::Whitespaces:
            source.Next();
            return SingleParameter{key, Value(source)};
        default:
            throw UnexpectedValueException(
                source.GetParams(),
                source.GetBounds(source.GetCurrent().value)
            );
    }
}

std::map<std::string, std::string> Params(Source &source)
{
    std::map<std::string, std::string> result;

    while (true) {
        SkipWhitespaces(source);
        switch (source.GetCurrent().token) {
            case Token::End:
                return result;
            case Token::Slash: {
                const SingleParameter param = Param(source);
                auto [_, is_inserted] = result.try_emplace(std::string(param.key), std::string(param.value));
                if (!is_inserted) {
                    auto bounds =
                        source.GetBounds(param.key);
                    bounds.begin--;
                    throw SpecifiedTwiceParameterException(
                        source.GetParams(),
                        bounds
                    );
                }
                break;
            }
            default:
                const auto begin = source.GetBounds(source.GetCurrent().value).begin;
                const auto value = Value(source);
                const auto end = source.GetBounds(source.GetCurrent().value).begin;
                throw UnexpectedValueException(source.GetParams(),
                                               {begin, end});
        }


    }
}

}

std::map<std::string, std::string> ParseParams(const std::string &params)
{
    /*
     * Params -> Param*
     * Param -> Token::WhiteSpaces? <> Key <> (Token::Whitespaces <> Value)?
     * Key -> Token::Slash <> Token::Word
     * Value -> NoQuotedText | QuotedText
     * NoQuotedText -> ((Token::EscapedSequence | Token::Word) <> (Token::EscapedSequence | Token::Word | Token::Slash)*)?
     * QuotedText
     *      -> Token::Quote
     *      <>  (Token::EscapedSequence | Token::Word | Token::Slash | Token::Whitespaces)*
     *      <>  Token::Quote
     */
    Source source(params);
    return parse::Params(source);
}

