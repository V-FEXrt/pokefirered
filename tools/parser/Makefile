CXX := g++

CXXFLAGS := -Wall -std=c++11 -O2

INCLUDES := -I .

SRCS := main.cpp parser.cpp lexer.cpp

HEADERS := lexer.h parser.h 

.PHONY: all clean

all: parser
	@:

parser: $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $@ $(LDFLAGS)

clean:
	$(RM) jsonproc jsonproc.exe
