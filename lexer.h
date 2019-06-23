#pragma once
#include "error.h"
#include "scanner.h"
#include "token.h"
#include <cctype>
#include <memory>
#include <string>

namespace akan {
class Lexer {
private:
  std::shared_ptr<Scanner> scanner_;
  char ch_ = ' ';
  std::shared_ptr<Token> token_;

  void SkipWhiteSpace() {
    while (ch_ == ' ' || ch_ == '\n' || ch_ == '\t') {
      ch_ = scanner_->Scan();
    }
  }

  bool Scan(char need) {
    ch_ = scanner_->Scan();
    if (ch_ == need)
      return true;
    else
      return false;
  }

  bool IsHexChar(char ch) {
    return std::isdigit(ch) || (ch >= 'A' && ch <= 'F') ||
           (ch >= 'a' && ch <= 'f');
  }

  void Scan() {
    ch_ = scanner_->Scan();
    return;
  }

  void TokenizeIdentifierOrKeywords() {
    std::string name;
    do {
      name.push_back(ch_);
      // Eat one more character here
      ch_ = scanner_->Scan();
    } while (std::isalpha(ch_) || ch_ == '_');
    if (Keyword::IsKeyword(name)) {
      token_ = std::make_shared<Keyword>(name);
    } else {
      token_ = std::make_shared<Identifier>(name);
    }
    return;
  }

  void TokenizeString() {
    std::string str;
    while (!Scan('"')) {
      if (ch_ == '\\') {
        Scan();
        switch (ch_) {
        case 'n':
          str.push_back('\n');
          break;
        case '\\':
          str.push_back('\\');
          break;
        case 't':
          str.push_back('\t');
          break;
        case '"':
          str.push_back('"');
          break;
        case '0':
          str.push_back('\0');
          break;
        case '\n':
          break;
        case -1:
          // Eat one more character here
          Error::PrintLexicalError(STR_NO_R_QUOTE);
          token_ = std::make_shared<Token>(ERR);
          return;
        default:
          str.push_back(ch_);
        }
      } else if (ch_ == '\n' || ch_ == -1) {
        // Eat one more character here
        Error::PrintLexicalError(STR_NO_R_QUOTE);
        token_ = std::make_shared<Token>(ERR);
        return;
      } else {
        str.push_back(ch_);
      }
    }
    token_ = std::make_shared<String>(str);
    // Eat one more character here
    Scan();
  }

  void TokenizeNumber() {
    int val = 0;
    // Decimal
    if (ch_ != 0) {
      do {
        val = val * 10 + ch_ - '0';
        // Eat one more character here
        Scan();
      } while (std::isdigit(ch_));
    } else {
      Scan();
      // Hexadecimal
      if (ch_ == 'x') {
        Scan();
        if (IsHexChar(ch_)) {
          do {
            val = val * 16 + ch_;
            if (std::isdigit(ch_))
              val -= '0';
            else if (ch_ >= 'A' && ch_ <= 'F')
              val += 10 - 'A';
            else if (ch_ >= 'a' && ch_ <= 'f')
              val += 10 - 'a';
            // Eat one more character here
            Scan();
          } while (IsHexChar(ch_));
        } else {
          // Eat one more character here
          Error::PrintLexicalError(HEX_NUM_NO_ENTITY);
          token_ = std::make_shared<Token>(ERR);
          return;
        }
      }
      // Binary
      else if (ch_ == 'b') {
        Scan();
        if (ch_ >= '0' && ch_ <= '1') {
          do {
            val = val * 2 + ch_ - '0';
            // Eat one more character here
            Scan();
          } while (ch_ >= '0' && ch_ <= '1');
        } else {
          // Eat oone more character here
          Error::PrintLexicalError(BI_NUM_NO_ENTITY);
          token_ = std::make_shared<Token>(ERR);
          return;
        }
      }
      // Octal
      else if (ch_ >= '0' && ch_ <= '7') {
        do {
          val = val * 8 + ch_ - '0';
          // Eat one more character here
          Scan();
        } while (ch_ >= '0' && ch_ <= '7');
      }
    }
    token_ = std::make_shared<Number>(val);
  }

  void TokenizeCharacter() {
    Scan();
    char c;
    // Escape character
    if (ch_ = '\\') {
      Scan();
      if (ch_ == 'n')
        c = '\n';
      else if (ch_ == '\\')
        c = '\\';
      else if (ch_ == 't')
        c = '\t';
      else if (ch_ == '0')
        c = '\0';
      else if (ch_ == '\'')
        c = '\'';
      // End of file or line break
      else if (ch_ == -1 || ch_ == '\n') {
        // Eat one more character here
        Error::PrintLexicalError(CHAR_NO_R_QUOTE);
        token_ = std::make_shared<Token>(ERR);
        return;
      }
      // Non-escape character
      else
        c = ch_;
    } else if (ch_ == -1 || ch_ == '\n') {
      // Eat one more character here
      Error::PrintLexicalError(CHAR_NO_R_QUOTE);
      token_ = std::make_shared<Token>(ERR);
      return;
    }
    // No entity
    else if (ch_ == '\'') {
      // Eat one more character here
      Error::PrintLexicalError(NOT_SUPPORT_NULL_CHAR);
      token_ = std::make_shared<Token>(ERR);
      return;
    }
    // Non-escape character
    else {
      c = ch_;
    }
    if (Scan('\'')) {
      token_ = std::make_shared<Character>(c);
      // Eat one more character here
      Scan();
      return;
    } else {
      // Eat one more character here
      Error::PrintLexicalError(CHAR_NO_R_QUOTE);
      token_ = std::make_shared<Token>(ERR);
      return;
    }
  }

  void TokenizeDelimiter() {
    switch (ch_) {
    // Ignore  macro
    case '#':
      while (ch_ != '\n' && ch_ != -1)
        // Eat one more character here
        Scan();
      break;
    case '+':
      token_ = std::make_shared<Delimiter>(Scan('+') ? INC : ADD);
      // Eat one more character here
      Scan();
      break;
    case '-':
      token_ = std::make_shared<Delimiter>(Scan('-') ? DEC : SUB);
      // Eat one more character here
      Scan();
      break;
    case '*':
      token_ = std::make_shared<Delimiter>(MUL);
      // Eat one more character here
      Scan();
      break;
    case '/':
      Scan();
      // Single-line comment
      if (ch_ == '/') {
        while (ch_ != '\n' && ch_ != -1) {
          // Eat one more character here
          Scan();
        }
        // make pointer null if match a comment
        token_ = nullptr;
        return;
      }
      // Multi-line comment
      else if (ch_ == '*') {
        while (!Scan(-1)) {
          if (ch_ == '*') {
            if (Scan('/'))
              break;
          }
        }
        if (ch_ == -1) {
          // Eat one more character here
          Error::PrintLexicalError(COMMENT_NO_END);
          token_ = std::make_shared<Token>(ERR);
          return;
        } else {
          // Eat one more character here
          Scan();
          // make pointer null if match a comment
          token_ = nullptr;
          return;
        }
      }
      // Division operator
      else {
        token_ = std::make_shared<Delimiter>(DIV);
        // Eat one more character here
        Scan();
        return;
      }
    case '%':
      token_ = std::make_shared<Delimiter>(MOD);
      // Eat one more character here
      Scan();
      break;
    case '>':
      token_ = std::make_shared<Delimiter>(Scan('=') ? GE : GT);
      // Eat one more character here
      Scan();
      break;
    case '<':
      token_ = std::make_shared<Delimiter>(Scan('=') ? LE : LT);
      // Eat one more character here
      Scan();
      break;
    case '=':
      token_ = std::make_shared<Delimiter>(Scan('=') ? EQU : ASSIGN);
      // Eat one more character here
      Scan();
      break;
    case '&':
      token_ = std::make_shared<Delimiter>(Scan('&') ? AND : LEA);
      // Eat one more character here
      Scan();
      break;
    case '|':
      if (Scan('|')) {
        token_ = std::make_shared<Delimiter>(OR);
        // Eat one more character here
        Scan();
        return;
      } else {
        token_ = std::make_shared<Token>(ERR);
        // Eat one more character here
        Error::PrintLexicalError(OR_NO_PAIR);
        return;
      }
    case ',':
      token_ = std::make_shared<Delimiter>(COMMA);
      // Eat one more character here
      Scan();
      break;
    case ':':
      token_ = std::make_shared<Delimiter>(COLON);
      // Eat one more character here
      Scan();
      break;
    case ';':
      token_ = std::make_shared<Delimiter>(SEMICON);
      // Eat one more character here
      Scan();
      break;
    case '(':
      token_ = std::make_shared<Delimiter>(LPAREN);
      // Eat one more character here
      Scan();
      break;
    case ')':
      token_ = std::make_shared<Delimiter>(RPAREN);
      // Eat one more character here
      Scan();
      break;
    case '[':
      token_ = std::make_shared<Delimiter>(LBRACK);
      // Eat one more character here
      Scan();
      break;
    case ']':
      token_ = std::make_shared<Delimiter>(RBRACK);
      // Eat one more character here
      Scan();
      break;
    case '{':
      token_ = std::make_shared<Delimiter>(LBRACE);
      // Eat one more character here
      Scan();
      break;
    case '}':
      token_ = std::make_shared<Delimiter>(RBRACE);
      // Eat one more character here
      Scan();
      break;
    case -1:
      token_ = std::make_shared<Token>(END);
      break;
    default:
      token_ = std::make_shared<Token>(ERR);
      // Eat one more character here
      Error::PrintLexicalError(TOKEN_NO_EXIST);
    }
  }

public:
  Lexer(std::shared_ptr<Scanner> scanner) : scanner_(scanner) {
    Error::SetScanner(scanner);
  }
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  ~Lexer() = default;
  // All Tokenize function should eat one more character except that an error
  // occurs or scanner reaches the end of the file.
  std::shared_ptr<Token> Tokenize() {
    // Use a loop here is to skip the comment and print out all lexical error.
    do {
      SkipWhiteSpace();
      if (std::isalpha(ch_) || ch_ == '_')
        TokenizeIdentifierOrKeywords();
      else if (ch_ == '"')
        TokenizeString();
      else if (std::isdigit(ch_))
        TokenizeNumber();
      else if (ch_ == '\'')
        TokenizeCharacter();
      else
        TokenizeDelimiter();
      if (token_ && token_->GetTag() != ERR)
        return token_;
    } while (ch_ != -1);
    return std::make_shared<Token>(END);
  }

private:
  // Debug helper
  static void TestImpl(const char *file_name) {
    Lexer lexer(std::make_shared<Scanner>(file_name));
    std::shared_ptr<Token> token;
    do {
      token = lexer.Tokenize();
      std::printf("%10s\t", Token::GetTagName(token->GetTag()).c_str());
      std::fflush(stdout);
      std::printf("%20s\n", token->ToString().c_str());
      std::fflush(stdout);
    } while (token->GetTag() != END);
    std::printf("Finish the lex for %s\n", file_name);
  }

public:
  static void MainTest(int argc = 0, char *argv[] = nullptr) {
    TestImpl("file/arithmetic.c");
    printf("\n");
    TestImpl("file/intended_error.c");
  }
};
} // namespace akan
