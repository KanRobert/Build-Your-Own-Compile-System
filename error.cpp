#include "error.h"
#include "scanner.h"

namespace akan {
std::shared_ptr<Scanner> Error::scanner_ = nullptr;
int Error::error_num_ = 0;
int Error::warn_num_ = 0;

const char *lexical_error_name[] = {"String misses right double quote.",
                                    "Binary number has no entity data.",
                                    "Hexadecimal number has no entity data.",
                                    "Character misses right single quote.",
                                    "Not support null character.",
                                    "Or operator should be double &.",
                                    "Multi-line comment does not end normally.",
                                    "Lexical notation does not exist."};

void Error::PrintLexicalError(int code) {
  IncrErrorNum();
  printf("%s<line %d, col %d> LexicalError: %s.\n", scanner_->GetFile(),
         scanner_->GetLine(), scanner_->GetCol(), lexical_error_name[code]);
}

void Error::PrintSyntaxError(int code, std::shared_ptr<Token> token){
}

} // namespace akan
