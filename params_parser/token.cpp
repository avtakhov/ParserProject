#include "token.h"
#include "parser_exceptions.h"

#include <algorithm>
#include <string>

namespace
{

const std::unordered_map<std::string, Token> FIXED_SIZE_TOKENS = {
    {"/", Token::Slash},
    {"\\\\", Token::EscapedSequence},
    {"\\\"", Token::EscapedSequence},
    {"\\/", Token::EscapedSequence},
    {"\\ ", Token::EscapedSequence},
    {"\"", Token::Quote},
};

std::optional<View> ReadFixedSize(std::string_view from)
{
    for (const auto &[view, token] : FIXED_SIZE_TOKENS) {
        if (from.starts_with(view)) {
            return View{token, from.substr(0, view.size())};
        }
    }
    return std::nullopt;
}

std::optional<View> ReadEnd(std::string_view from)
{
    if (from.empty()) {
        return View{Token::End, from};
    }
    return std::nullopt;
}

std::optional<View> ReadUntil(std::string_view from, Token token, const std::function<bool(char)> &predicate)
{
    auto slice =
        std::string_view(from.begin(), std::ranges::find_if_not(from, predicate));
    if (slice.empty()) {
        return std::nullopt;
    }
    return View{token, slice};
}

std::optional<View> ReadWhitespaces(std::string_view from)
{
    return ReadUntil(from, Token::Whitespaces, static_cast<int (*)(int)>(&std::isspace));
}

std::optional<View> ReadWord(std::string_view from)
{
    std::size_t word_length = 0;
    while (true) {
        std::string_view after = from.substr(word_length, from.length() - word_length);
        if (ReadEnd(after) || ReadFixedSize(after) || ReadWhitespaces(after)) {
            return View{Token::Word, from.substr(0, word_length)};
        }

        word_length++;
    }
}

std::optional<View> ReadTokenFrom(std::string_view source)
{
    if (const auto end = ReadEnd(source)) return end;
    if (const auto fixed_size = ReadFixedSize(source)) return fixed_size;
    if (const auto whitespaces = ReadWhitespaces(source)) return whitespaces;
    if (const auto word = ReadWord(source)) return word;

    return std::nullopt;
}

}

ParamsChunk Source::GetBounds(std::string_view sub_view) const
{
    const std::size_t begin = std::distance(m_params.data(), sub_view.data());
    return ParamsChunk{begin, begin + sub_view.size()};
}

void Source::Next()
{
    const auto view = ReadTokenFrom(m_rest);
    if (!view.has_value()) {
        throw UnexpectedValueException(m_params, GetBounds(m_rest));
    }
    m_current = view.value();
    m_rest = std::string_view(std::next(m_rest.begin(), m_current.value.size()), m_rest.end());
}

View Source::GetCurrent() const
{
    return m_current;
}

Source::Source(std::string_view params)
    : m_rest(params), m_params(params)
{
    Next();
}

View Source::ExpectOneOf(const std::unordered_set<Token> &tokens)
{
    if (!CheckOneOf(tokens)) {
        throw UnexpectedValueException(m_params, GetBounds(m_current.value));
    }
    const auto current = GetCurrent();
    Next();
    return current;
}

std::string_view Source::GetParams() const
{
    return m_params;
}

bool Source::CheckOneOf(const std::unordered_set<Token> &tokens) const
{
    return tokens.contains(GetCurrent().token);
}

ParamsChunk Source::ToEnd(std::string_view from) const
{
    auto bounds = GetBounds(from);
    return {bounds.begin, m_params.size()};
}

std::string View::ToString() const
{
    if (token == Token::EscapedSequence) {
        return std::string{std::next(value.begin()), value.end()};
    }
    return std::string(value);
}
