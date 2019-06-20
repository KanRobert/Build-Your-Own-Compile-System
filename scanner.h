#pragma once
#include "error.h"
#include <memory>
//#define DEBUG

namespace akan {
class Scanner {

  static void CloseFile(std::FILE *fp) {
    if (fp) {
      std::fclose(fp);
    }
  }
  // File
  const char *file_name_ = nullptr;
  std::unique_ptr<FILE, decltype(CloseFile) *> file_;

  // Characters read
  static constexpr int buf_len_ = 80; // Length of scan buffer
  char line_[buf_len_];

  // Read status
  int line_len_ = 0;  // Length of current line
  int read_pos_ = -1; // Read position
  char last_ch_ = 0;  // Last character, used to judge the line break position
  int line_num_ = 1;  // Row Number
  int col_num_ = 0;   // Column Number

#ifdef DEBUG
  // Show character
  static void ShowChar(char ch) {
    switch (ch) {
    case -1:
      std::printf("EOF");
      break;
    case '\n':
      std::printf("\\n");
      break;
    case '\t':
      std::printf("\\t");
      break;
    case ' ':
      std::printf("<blank>");
      break;
    default:
      std::printf("%c", ch);
    }
    std::printf("\t\t<%d>", ch);
  }
#endif
public:
  Scanner(const char *name)
      : file_name_(name), file_(std::fopen(name, "r"), &CloseFile) {
    if (!file_) {
      PrintCommonError(
          FATAL, "Fail to open the file %s! Please check filename and path.\n",
          name);
      Error::IncrErrorNum();
    }
  }

  Scanner(const Scanner &) = delete;
  Scanner &operator=(const Scanner &) = delete;
  ~Scanner() = default;

  // Scan characters from  buffer
  int Scan() {
    if (!file_)
      return -1;
    if (read_pos_ == line_len_ - 1) {
      line_len_ = fread(line_, 1, buf_len_, file_.get()); // reload buffer data
      if (line_len_ == 0) {                               // no data
        // indicate end of file
        line_len_ = 1;
        line_[0] = -1;
        return -1;
      }
      read_pos_ = -1; // restore reading position
    }
    ++read_pos_;
    char ch = line_[read_pos_]; // get the new char
    if (last_ch_ == '\n') {     // start new line
      ++line_num_;
      col_num_ = 0;
    } else {
      ++col_num_;
    }
    last_ch_ = ch;
#ifdef DEBUG
    ShowChar(ch);
#endif
    return ch;
  }

  // Getter
  const char *GetFile() const { return file_name_; }
  int GetLine() const { return line_num_; }
  int GetCol() const { return col_num_; }

  static void MainTest(int argc = 0, char *argv[] = nullptr) {
    const char file_name[] = "file/arithmetic.c";
    Scanner scanner(file_name);
    while (scanner.Scan() != -1) {
      std::printf("\n");
    }
  }
};
} // namespace akan
