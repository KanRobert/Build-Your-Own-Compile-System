#include "token.h"
namespace akan {
std::array<const char *, 48> Token::tag_name_ = {
    "Error",  "End of file", "Identifier", "int",    "char",     "void",
    "extern", "Number",      "Character",  "String", "!",        "&",
    "+",      "-",           "*",          "/",      "%",        "++",
    "--",     ">",           ">=",         "<",      "<=",       "==",
    "!=",     "&&",          "||",         "(",      ")",        "[",
    "]",      "{",           "}",          ",",      ":",        ";",
    "=",      "if",          "else",       "switch", "else",     "default",
    "while",  "do",          "for",        "break",  "continue", "return"};

std::unordered_map<std::string, Tag> Keyword::keywords_ = {
    {"int", KW_INT},         {"char", KW_CHAR},
    {"void", KW_VOID},       {"extern", KW_EXTERN},
    {"if", KW_IF},           {"else", KW_ELSE},
    {"switch", KW_SWITCH},   {"case", KW_CASE},
    {"default", KW_DEFAULT}, {"while", KW_WHILE},
    {"do", KW_DO},           {"for", KW_FOR},
    {"break", KW_BREAK},     {"continue", KW_CONTINUE},
    {"return", KW_RETURN}};

std::string Token::ToString() const { return tag_name_[tag_]; }

std::string Keyword::ToString() const { return "[Keyword]: " + name_; }

std::string Delimiter::ToString() const { return "[Delimiter]: " + name_; }

std::string Identifier::ToString() const {
  return "[" + Token::ToString() + "]: " + name_;
}

std::string String::ToString() const {
  return "[" + Token::ToString() + "]: " + content_;
}

std::string Number::ToString() const {
  return "[" + Token::ToString() + "]: " + std::to_string(value_);
}

std::string Character::ToString() const {
  return "[" + Token::ToString() + "] " + std::string(1, ch_);
}

bool Keyword::IsKeyword(const std::string &name) {
  return keywords_.find(name) != keywords_.end();
}
} // namespace akan
