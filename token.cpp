#include "token.h"
namespace akan {
enum Tag {
  ERR,
  END,
  ID,
  KW_INT,
  KW_CHAR,
  KW_VOID,
  KW_EXTERN,
  NUM,
  CH,
  STR,
  NOT,
  LEA,
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  INC,
  DEC,
  GT,
  GE,
  LT,
  LE,
  EQU,
  NEQU,
  AND,
  OR,
  LPAREN,
  RPAREN,
  LBRACK,
  RBRACK,
  LBRACE,
  RBRACE,
  COMMA,
  COLON,
  SEMICON,
  ASSIGN,
  KW_IF,
  KW_ELSE,
  KW_SWITCH,
  KW_CASE,
  KW_DEFAULT,
  KW_WHILE,
  KW_DO,
  KW_FOR,
  KW_BREAK,
  KW_CONTINUE,
  KW_RETURN
};
}
const char *tag_name[] = {
    "Error",  "End of file", "Identifier", "int",    "char",     "void",
    "extern", "Number",      "Character",  "String", "!",        "&",
    "+",      "-",           "*",          "/",      "%",        "++",
    "--",     ">",           ">=",         "<",      "<=",       "==",
    "!=",     "&&",          "||",         "(",      ")",        "[",
    "]",      "{",           "}",          ",",      ":",        ";",
    "=",      "if",          "else",       "switch", "else",     "default",
    "while",  "do",          "for",        "break",  "continue", "return"};

virtual std::string Token::ToString() const { return tag_name[tag_]; }
virtual std::string Id::ToString() const {
  return "[" + Token::ToString() + "]:" + name_;
}
virtual std::string Str::ToString() const {
  return "[" + Token::ToString() + "]:" + str_;
}
virtual std::string Num::ToString() const {
  return "[" + Token::ToString() + "]:" + std::to_string(val_);
}

virtual std::string Char::ToString() const {
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
