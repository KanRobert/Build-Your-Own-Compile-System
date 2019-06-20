#include "token.h"
namespace akan {
  std::array<const char *,48>tag_name = {
    "Error",  "End of file", "Identifier", "int",    "char",     "void",
    "extern", "Number",      "Character",  "String", "!",        "&",
    "+",      "-",           "*",          "/",      "%",        "++",
    "--",     ">",           ">=",         "<",      "<=",       "==",
    "!=",     "&&",          "||",         "(",      ")",        "[",
    "]",      "{",           "}",          ",",      ":",        ";",
    "=",      "if",          "else",       "switch", "else",     "default",
    "while",  "do",          "for",        "break",  "continue", "return"};

std::string Token::ToString() const { return tag_name[tag_]; }
std::string Id::ToString() const {
  return "[" + Token::ToString() + "]:" + name_;
}
std::string Str::ToString() const {
  return "[" + Token::ToString() + "]:" + str_;
}
std::string Num::ToString() const {
  return "[" + Token::ToString() + "]:" + std::to_string(val_);
}

std::string Char::ToString() const {
  return "[" + Token::ToString() + "]" + std::string(1, ch_);
}

std::unordered_map<std::string, Tag> keywords = {
    {"int", KW_INT},         {"char", KW_CHAR},
    {"void", KW_VOID},       {"extern", KW_EXTERN},
    {"if", KW_IF},           {"else", KW_ELSE},
    {"switch", KW_SWITCH},   {"case", KW_CASE},
    {"default", KW_DEFAULT}, {"while", KW_WHILE},
    {"do", KW_DO},           {"for", KW_FOR},
    {"break", KW_BREAK},     {"continue", KW_CONTINUE},
    {"return", KW_RETURN}};

Tag GetTag(const std::string &name) {
  return (keywords.find(name) != keywords.end()) ? keywords[name] : ID;
}
} // namespace akan
