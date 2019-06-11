#pragma once
#include <cstdio>
#include <string>
#include <utility>

namespace akan {
enum CommonError;
enum LexicalError;
enum SyntaxError;
enum SemanticError;

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
  static Scanner *scanner_;
  static int error_num_;
  static int warn_num_;

public:
  Error(Scaner *scanner) { scanner_ = scanner; }
  Error(const Error &) = delete;
  Error &operator=(const Error &) = delete;
  ~Error() = default;

  static void IncrErrorNum() { ++error_num_; }
  static void IncrWarnNum() { ++warn_num_; }
  static int GetErrorNum(){return error_num_};
  static int GetWarnNum(){return warn_num_};

  static void PrintLexicalError(int code);
  static void PrintSyntaxError(int code, Token *t);
  static void PrintSemanticError(int code, const std::string &name = "");
  static void PrintSemanticWarning(int code, const std::string &name = "");
}
} // namespace akan
