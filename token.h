#pragma once
#include <string>
#include<unordered_map>

namespace akan {
enum Tag;
class Token {
public:
  Tag tag_;
  Token(Tag tag) : tag_(tag) {}
  Token(const Token &) = default;
  Token &operator(const Token &) = default;
  virtual std::string ToString() const;
  virtual ~Token() = default;
}

class Id : public Token {
public:
  std::string name_;
  Id(const string &name) : Token(ID), name_(name) {}
  Id(const Id &) = default;
  Id &operator(const Id &) = default;
  virtual std::string ToString() const override;
  virtual ~Id() = default;
}

class Str : public Token {
public:
  std::string str_;
  Str(const string &str) : Token(STR), str_(str) {}
  Str(const Str &) = default;
  Str &operator(const Str &) = default;
  virtual std::string ToString() const override;
  virtual ~Str() = default;
}

class Num : public Token {
public:
  int val_;
  Num(int val) : Token(NUM), val_(val) {}
  Num(const Num &) = default;
  Num &operator(const Num &) = default;
  virtual std::string ToString() const override;
  virtual ~Num() = default;
}

class Char : public Token {
public:
  char ch_;
  Char(char ch) : Token(CH), ch_(ch) {}
  Char(const Char &) = default;
  Char &operator(const Char &) = default;
  virtual std::string ToString() const override;
  virtual ~Char() = default;
}

extern std::unordered_map<std::string,Tag> keywords;
// test whether it is a keyword or ID
Tag GetTag(const std::string &name);
} // namespace akan
