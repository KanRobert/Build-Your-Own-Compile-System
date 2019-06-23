#pragma once
#include "lexer.h"
#include <cstdio>
#include <initializer_list>
#include <memory>
#define PARSER_DEBUG

namespace skan {
class Parser {
private:
  std::shared_ptr<Lexer> lexer_;
  std::shared_ptr<Token> token_;
  std::shared_ptr<SymbolTable> symbol_table_;
  std::shared_ptr<IRGenerator> ir_generator_;

  void Move() {
    token_ = lexer_.Tokenize();
#ifdef PARSER_DEBUG
    std::printf("%s\n", token_->ToString().c_str());
    std::fflush(stdout);
#endif
  }

  bool Match(Tag tag) { return token_->GetTag() == tag; }

  bool MatchThenMove(Tag tag) {
    if (Match(tag)) {
      Move();
      return true;
    } else {
      return false;
    }
  }

  bool IsType() { return Token::IsType(token_->GetTag()); }
  bool IsTag(std::initializer_list<Tag> tags) {
    for (auto tag : tags) {
      if (token_->GetTag() == tag)
        return true;
    }
    return false;
  }

  void RecoverFromError(bool condition, SyntaxError lost_error,
                        SyntaxError wrong_error) {
    if (condition) {
      PrintSyntaxError(lost_error, token_);
    } else {
      PrintSyntaxError(wrong_error, token_);
      Move();
    }
  }

  // <program>-><segment><program>
  void ParseProgram() {
    if (Match(END)) {
      return;
    } else {
      ParseSegment();
      ParseProgram();
    }
  }

  // <segment>->kw_extern <type><def> | <type><def>
  void ParseSegment() {
    bool has_extern = MatchThenMove(KW_EXTERN);
    Tag tag = ParseType();
    ParseDef(has_extern, tag);
  }

  // <type>->kw_int | kw_char | kw_void
  Tag ParseType() {
    if (IsType()) {
      Move();
      return token_->GetTag();
    } else {
      RecoverFromError(IsTag({ID,MUL}),TYPE_LOST, TYPE_WRONG);
      return KW_INT;
    }
  }

public:
  Parser(const Parser &) = delete;
  Parser &operator=(const Parser &) = delete;
  ~Parser = default;

  Parser(std::shared_ptr<Lexer> lexer,
         std::shared_ptr<SymbolTable> symbol_table,
         std::shared_ptr<IRGenerator> ir_generator)
      : lexer_(lexer), symbol_table_(symbol_table),
        ir_generator_(ir_generator) {}

  void Parse() {
    Move();
    ParseProgram();
  }
};
} // namespace skan
