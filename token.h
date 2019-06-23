#pragma once
#include <array>
#include <string>
#include <unordered_map>

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
class Token {
private:
  Tag tag_;

protected:
  static std::array<const char *, 48> tag_name_;

public:
  Tag GetTag() { return tag_; }
  static std::string GetTagName(Tag tag) { return tag_name_[tag]; }
  static bool IsType(Tag tag) {
    return tag == KW_INT || tag == KW_CHAR || tag == KW_VOID;
  }
  Token(Tag tag) : tag_(tag) {}
  Token(const Token &) = default;
  Token &operator=(const Token &) = default;
  virtual std::string ToString() const;
  virtual ~Token() = default;
};

class Keyword : public Token {
  std::string name_;
  static std::unordered_map<std::string, Tag> keywords_;

public:
  std::string GetName() { return name_; }
  Keyword(const std::string &name) : Token(keywords_[name]), name_(name) {}
  Keyword(const Keyword &) = default;
  Keyword &operator=(const Keyword &) = default;
  virtual std::string ToString() const override;
  virtual ~Keyword() = default;
  static bool IsKeyword(const std::string &name);
};

class Delimiter : public Token {
  std::string name_;

public:
  std::string GetName() { return name_; }
  Delimiter(Tag tag) : Token(tag), name_(tag_name_[tag]) {}
  Delimiter(const Delimiter &) = default;
  Delimiter &operator=(const Delimiter &) = default;
  virtual std::string ToString() const override;
  virtual ~Delimiter() = default;
};

class Identifier : public Token {
  std::string name_;

public:
  std::string GetName() { return name_; }
  Identifier(const std::string &name) : Token(ID), name_(name) {}
  Identifier(const Identifier &) = default;
  Identifier &operator=(const Identifier &) = default;
  virtual std::string ToString() const override;
  virtual ~Identifier() = default;
};

class String : public Token {
  std::string content_;

public:
  std::string GetContent() { return content_; }
  String(const std::string &content) : Token(STR), content_(content) {}
  String(const String &) = default;
  String &operator=(const String &) = default;
  virtual std::string ToString() const override;
  virtual ~String() = default;
};

class Number : public Token {
  int value_;

public:
  int GetValue() { return value_; }
  Number(int value) : Token(NUM), value_(value) {}
  Number(const Number &) = default;
  Number &operator=(const Number &) = default;
  virtual std::string ToString() const override;
  virtual ~Number() = default;
};

class Character : public Token {
  char ch_;

public:
  char GetContent() { return ch_; }
  Character(char ch) : Token(CH), ch_(ch) {}
  Character(const Character &) = default;
  Character &operator=(const Character &) = default;
  virtual std::string ToString() const override;
  virtual ~Character() = default;
};

} // namespace akan
