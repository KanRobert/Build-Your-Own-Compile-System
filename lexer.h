#include "error.h"
#include "scanner.h"
#include "token.h"
#include <cctype>
#include <memory>
#include <string>

namespace akan {
class Lexer {
private:
  std::unique_ptr<Scanner> scanner_;
  char ch_ = ' ';
  std::unique_ptr<Token> token_;
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

  void TokenizeIdOrKeywords() {
    std::string name;
    do {
      name.push_back(ch_);
      // Eat one more character here
      ch_ = scanner_->Scan();
    } while (std::isalpha(ch_) || ch_ == '_');
    Tag tag = GetTag(name);
    if (tag == ID)
      token_ = std::make_unique<Id>(name);
    else
      token_ = std::make_unique<Token>(tag);
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
          // Ear one more character here
          Error::PrintLexicalError(STR_NO_R_QUOTE);
          token_ = std::make_unique<Token>(ERR);
          return;
        default:
          str.push_back(ch_);
        }
      } else if (ch_ == '\n' || ch_ == -1) {
        // Eat one more character here
        Error::PrintLexicalError(STR_NO_R_QUOTE);
        token_ = std::make_unique<Token>(ERR);
        return;
      } else {
        str.push_back(ch_);
      }
    }
    if (!token_) {
      token_ = std::make_unique<Str>(str);
      // Eat one more character here
      Scan();
    }
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
          token_ = std::make_unique<Token>(ERR);
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
          token_ = std::make_unique<Token>(ERR);
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
    if (!token_)
      token_ = std::make_unique<Num>(val);
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
        token_ = std::make_unique<Token>(ERR);
        return;
      }
      // Non-escape character
      else
        c = ch_;
    } else if (ch_ == -1 || ch_ == '\n') {
      // Eat one more character here
      Error::PrintLexicalError(CHAR_NO_R_QUOTE);
      token_ = std::make_unique<Token>(ERR);
      return;
    }
    // No entity
    else if (ch_ == '\'') {
      // Eat one more character here
      Error::PrintLexicalError(NOT_SUPPORT_NULL_CHAR);
      token_ = std::make_unique<Token>(ERR);
      return;
    }
    // Non-escape character
    else
      c = ch_;
    if (!token_) {
      if (Scan('\'')) {
        token_ = std::make_unique<Char>(c);
        // Eat one more character here
        Scan();
      } else {
        // Eat one more character here
        Error::PrintLexicalError(CHAR_NO_R_QUOTE);
        token_ = std::make_unique<Token>(ERR);
        return;
      }
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
      token_ = std::make_unique<Token>(Scan('+') ? INC : ADD);
      // Eat one more character here
      Scan();
      break;
    case '-':
      token_ = std::make_unique<Token>(Scan('-') ? DEC : SUB);
      // Eat one more character here
      Scan();
      break;
    case '*':
      token_ = std::make_unique<Token>(MUL);
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
          token_ = std::make_unique<Token>(ERR);
          return;
        }
        else{
          // Eat one more character here
          Scan();
          return;
        }
      }
      // Division operator
      else {
        token_ = std::make_unique<Token>(DIV);
        // Eat one more character here
        Scan();
        return;
      }
    case '%':
      token_ = std::make_unique<Token>(MOD);
      // Eat one more character here
      Scan();
      break;
    case '>':
      token_ = std::make_unique<Token>(Scan('=') ? GE : GT);
      // Eat one more character here
      Scan();
      break;
    case '<':
      token_ = std::make_unique<Token>(Scan('=') ? LE : LT);
      // Eat one more character here
      Scan();
      break;
    case '=':
      token_ = std::make_unique<Token>(Scan('=') ? EQU : ASSIGN);
      // Eat one more character here
      Scan();
      break;
    case '&':
      token_ = std::make_unique<Token>(Scan('&') ? AND : LEA);
      // Eat one more character here
      Scan();
      break;
    case '|':
      token_ = std::make_unique<Token>(Scan('|') ? OR : ERR);
      if (token_->tag_ == ERR){
        // Eat one more character here
        Error::PrintLexicalError(OR_NO_PAIR);
        return;
      }
      else{
        // Eat one more character here
        Scan();
        return;
      }
    case ',':
      token_ = std::make_unique<Token>(COMMA);
      // Eat one more character here
      Scan();
      break;
    case ':':
      token_ = std::make_unique<Token>(COLON);
      // Eat one more character here
      Scan();
      break;
    case ';':
      token_ = std::make_unique<Token>(SEMICON);
      // Eat one more character here
      Scan();
      break;
    case '(':
      token_ = std::make_unique<Token>(LPAREN);
      // Eat one more character here
      Scan();
      break;
    case ')':
      token_ = std::make_unique<Token>(RPAREN);
      // Eat one more character here
      Scan();
      break;
    case '[':
      token_ = std::make_unique<Token>(LBRACK);
      // Eat one more character here
      Scan();
      break;
    case ']':
      token_ = std::make_unique<Token>(RBRACK);
      // Eat one more character here
      Scan();
      break;
    case '{':
      token_ = std::make_unique<Token>(LBRACE);
      // Eat one more character here
      Scan();
      break;
    case '}':
      token_ = std::make_unique<Token>(RBRACE);
      // Eat one more character here
      Scan();
      break;
    case -1:
      break;
    default:
      token_ = std::make_unique<Token>(ERR);
      // Eat one more character here
      Error::PrintLexicalError(TOKEN_NO_EXIST);
    }
  }

public:
  Lexer(std::unique_ptr<Scanner> scanner) : scanner_(std::move(scanner)) {}
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  ~Lexer() = default;
  // All Tokenize function should eat one more character. If an error occurs,
  // function naturally will eat one more character. So in this condition, there
  // is no need to explicitly eat one more;
  std::unique_ptr<Token> Tokenize() {
    do {
      SkipWhiteSpace();
      if (std::isalpha(ch_) || ch_ == '_')
        TokenizeIdOrKeywords();
      else if (ch_ == '"')
        TokenizeString();
      else if (std::isdigit(ch_))
        TokenizeNumber();
      else if (ch_ == '\'')
        TokenizeCharacter();
      else
        TokenizeDelimiter();
      if (token_ && token_->tag_ != ERR)
        return std::move(token_);
    } while (ch_ != -1);
    return std::make_unique<Token>(END);
  }

  static void MainTest(int argc = 0, char *argv[] = nullptr) {
    const char file_name[] = "file/arithmetic.c";
    Lexer lexer(std::make_unique<Scanner>(file_name));
    for (auto token = lexer.Tokenize(); token->tag_ != END;
         token = lexer.Tokenize()) {
      std::printf("%d\t", token->tag_);
      fflush(stdout);
      std::printf("%s\t", tag_name[token->tag_]);
      fflush(stdout);
      std::printf("%s\n", token->ToString().c_str());
      fflush(stdout);
    }
  }
};
} // namespace akan
