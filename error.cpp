#include "error.h"

namespace akan {
Scanner *Error::scanner_ = nullptr;
int Error::error_num_ = 0;
int Error::warn_num_ = 0;

enum CommonError { FATAL, ERROR, WARN };
enum LexicalError {
  STR_NO_R_QUOTE,
  BI_NUM_NO_ENTITY,
  HEX_NUM_NO_ENTITY,
  CHAR_NO_R_QUOTE,
  OR_NO_PAIR,
  COMMENT_NO_END,
  TOEKN_NO_EXIST
};

const char *lexical_error_name[]={
  "String misses right double quote.", "Binary number has no entity data.",
  "Hexadecimal number has no entity data.", "Character misses right single quote.",
  "Not support null character.","Multi-line comment does not end normally.",
  "Lexical notation does not exist."
}

void Error::PrintLexicalError(int code){
 IncrErrorNum();
 printf("%s<line %d, col %d> LexicalError: %s.\n",scanner_->GetFile(),
        scanner_->GetLine(),scanner_->GetCol(),lexical_error_name[code]); 
}

} // namespace akan
