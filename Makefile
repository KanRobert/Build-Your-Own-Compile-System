LEXER_OBJECTS = test_lexer.o token.o error.o
SCANNER_OBJECTS = test_scanner.o error.o

CXX = g++ -std=c++14 -g
EXE = test_lexer test_scanner

test_lexer :  $(LEXER_OBJECTS)
	$(CXX) -o test_lexer $(LEXER_OBJECTS)
test_scanner : $(SCANNER_OBJECTS)
	$(CXX) -o test_scanner $(SCANNER_OBJECTS)

test_lexer.o : lexer.h
test_scanner.o : scanner.h
error.o : error.h
token.o : token.h

lexer.h : error.h scanner.h token.h
scanner.h : error.h

.PHONY : clean
clean :
	-rm *.o $(EXE)
