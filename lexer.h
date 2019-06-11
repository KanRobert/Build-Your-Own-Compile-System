#include "scanner.h"
#include "token.h"
#include "error.h"
#include <memory>
#include <cctype>
#include <string>

namespace akan {
class Lexer {
private:
  Scanner *scanner_;
  char ch_ = ' ';
  std::unique_ptr<Token> token_;
  void SkipWhiteSpace() {
    while (ch_ == ' ' || ch_ == '\n' || ch_ == '\t') {
      ch_ = scanner_->Scan();
    }
  }

  bool Scan(char need){
    ch_=scanner->Scan();
    if(ch_==need)
      return true;
    else
      return false;
  }

  void Scan(){
    ch_=scanner->Scan();
    return;
  }

  void TokenizeIdOrKeywords(){
    std::string name;
    do{
      name.push_back(ch_);
      ch_=scanner_->Scan();
    }while(std::isalpha(ch_) || ch=='_');
    if(GetTag==ID)
      token_=make_unique<Id>(name);
    else
      token_=make_unique<Token>(tag);
  }

  void TokenizeString(){
    std::string str;
    while(!Scan('"')){
      if(ch_=='\\'){
        Scan();
        switch(ch_){
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
          case '-1':
            PrintLexicalError(STR_NO_R_QUOTE);
            token_=std::make_unique<Token>(ERR);
            return;
          default:
            str.push_back(ch);
        }
      }
      else if(ch_=='\n' || ch==-1){
        PrintLexicalError(STR_NO_R_QUOTE0);
        token_=std::make_unique<Token>(ERR);
        return;
      }
      else{
        str.push_back(ch);
      }
    }
    if(!t) token_=std::make_unique<Str>(str);
  }

  void TokenizeNumber(){
    int val=0;
    if(ch_!=0){
      do{
        val=val*10+ch_-'0';
        Scan();
      }while(is_digit(ch_))
    }
  }

public:
  Lexer(Scanner *scanner) : scanner_(scanner) Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;
  ~Lexer() = default;
  Token Tokenize() {
    do {
      SkipWhiteSpace();
      if(std::isalpha(ch_) || ch=='_')
        TokenizeIdOrKeywords();
      else if(ch=='"')
        TokenizeString();
      else if(std::isdigit(ch_))
        TokenizeNumber();
      else if(ch=='\'')
        TokenizeCharacter();
      else
        TokenizeDelimiter();
      if (token_ && token_->tag_ != ERR)
        return *token_;
    } while (ch != -1);
    return Token(END);
  }
}
} // namespace akan
