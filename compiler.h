namespace akan {
class Compiler {
  friend class Scanner;

private:
  static bool show_char_;   // show character
  static bool show_token_;  // show lexical mark
  static bool show_symtab_; // show symbol table
  static bool show_ir_;     // show intermediate representation
  static bool show_op_ir_;  // show optimized IR
  static bool show_block_;  // show basic block and control flow
  static bool show_help_;   // show help
  static bool optim_;       // whether to optimize
public:
  Compiler(const Compiler &) = delete;
  Compiler &operator(const Compiler &) = delete;
  ~Compiler() = delete;

  static void Compile(const char *file) {
    Scanner scanner(file);
    Error error(&scanner);
    Lexer lexer(scanner);
  }
}
