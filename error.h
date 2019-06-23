#pragma once
#include <cstdio>
#include <memory>
#include <string>
#include <utility>

namespace akan {
enum CommonError { FATAL, ERROR, WARN };
enum LexicalError {
  STR_NO_R_QUOTE,
  BI_NUM_NO_ENTITY,
  HEX_NUM_NO_ENTITY,
  CHAR_NO_R_QUOTE,
  NOT_SUPPORT_NULL_CHAR,
  OR_NO_PAIR,
  COMMENT_NO_END,
  TOKEN_NO_EXIST
};

enum SyntaxError {
  TYPE_LOST,
  TYPE_WRONG,
  ID_LOST,
  ID_WRONG,
  NUM_LOST,
  NUM_WRONG,
  LITERAL_LOST,
  LITERAL_WRONG,
  COMMA_LOST,
  COMMA_WRONG,
  SEMICON_LOST,
  SEMICON_WRONG,
  ASSIGN_LOST,
  ASSIGN_WRONG,
  COLON_LOST,
  COLON_WRONG,
  WHILE_LOST,
  WHILE_WRONG,
  LPAREN_LOST,
  LPAREN_WRONG,
  RPAREN_LOST,
  RPAREN_WRONG,
  LBRACK_LOST,
  LBRACK_WRONG,
  RBRACK_LOST,
  RBRACK_WRONG,
  LBRACE_LOST,
  LBRACE_WRONG,
  RBRACE_LOST,
  RBRACE_WRONG
};

enum SemanticError {
  VAR_RE_DEF,        // variable redefinition
  FUN_RE_DEF,        // function redefinition
  VAR_UN_DEC,        // variable not declared
  FUN_UN_DEC,        // function not declared
  FUN_DEC_ERR,       // function declaration is not consistent with definition
  FUN_CALL_ERR,      // function argument is not consistent with parameter
  DEC_INIT_DENY,     // declare variable with an initializer
  EXTERN_FUN_DEF,    // declare function with extern
  ARRAY_LEN_INVALID, // array length is invalid
  VAR_INIT_ERR,      // variable intializer has a wrong type
  GLB_INIT_ERR,      // global initializer is not constant value
  VOID_VAR,          // void variable
  EXPR_NOT_LEFT_VAL, // invalid left-value expression
  ASSIGN_TYPE_ERROR, // assign type dismatch
  EXPR_IS_BASE,      // expression is of a base type
  EXPR_NOT_BASE,     // expression is of not a base type
  ARR_TYPE_ERR,      // array type error
  EXPR_IS_VOID,      // expression is of a void type
  BREAK_ERR,         //  break occurs outside loop or switch-case
  CONTINUE_ERR,      // continue occurs outside a loop
  RETURN_ERR         // return type is no consistent with function type
};

template <typename... Args>
void PrintCommonError(CommonError common_error, Args &&... args) {
  switch (common_error) {
  case FATAL:
    printf("<fatal>:");
    printf(std::forward<Args>(args)...);
    break;
  case ERROR:
    printf("<error>:");
    printf(std::forward<Args>(args)...);
    break;
  case WARN:
    printf("<warn>:");
    printf(std::forward<Args>(args)...);
    break;
  }
}

class Scanner;
class Token;
class Error {
  static std::shared_ptr<Scanner> scanner_;
  static int error_num_;
  static int warn_num_;

public:
  Error(const Error &) = delete;
  Error &operator=(const Error &) = delete;
  ~Error() = delete;

  static void IncrErrorNum() { ++error_num_; }
  static void IncrWarnNum() { ++warn_num_; }
  static int GetErrorNum() { return error_num_; }
  static int GetWarnNum() { return warn_num_; }
  static void Clear() {
    scanner_ = nullptr;
    error_num_ = 0;
    warn_num_ = 0;
  }
  static void SetScanner(std::shared_ptr<Scanner> scanner) {
    scanner_ = scanner;
  }

  static void PrintLexicalError(int code);
  static void PrintSyntaxError(int code, std::shared_ptr<Token> token);
  static void PrintSemanticError(int code, const std::string &name = "");
  static void PrintSemanticWarning(int code, const std::string &name = "");
};
} // namespace akan
