#ifndef PARSERPROJECT_TOKENIZER_H
#define PARSERPROJECT_TOKENIZER_H

#include <string_view>
#include <string>
#include <optional>
#include <unordered_set>

#include "parser_exceptions.h"

enum class Token
{
    Quote,
    Slash,
    EscapedSequence,
    Word,
    Whitespaces,
    End,
};

struct View
{
    Token token;
    std::string_view value;

    [[nodiscard]] std::string ToString() const;
};

struct Source
{
    [[nodiscard]] View GetCurrent() const;

    [[nodiscard]] std::string_view GetParams() const;

    void Next();

    explicit Source(std::string_view params);

    [[nodiscard]] bool CheckOneOf(const std::unordered_set<Token> &tokens) const;

    View ExpectOneOf(const std::unordered_set<Token> &tokens);

    [[nodiscard]] ParamsChunk ToEnd(std::string_view from) const;

    ParamsChunk GetBounds(std::string_view sub_view) const;

private:
    View m_current;
    std::string_view m_rest;
    std::string_view m_params;
};


#endif //PARSERPROJECT_TOKENIZER_H
