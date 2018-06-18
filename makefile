all: recursive-parser

tmp/lex.yy.o: 3rd-party/lex.yy.cc
	g++ -O2 -g -c --std=c++11 3rd-party/lex.yy.cc -o tmp/lex.yy.o

tmp/main.o: main.cpp
	g++ -O2 -g -c --std=c++11 main.cpp -o tmp/main.o

recursive-parser: tmp/main.o tmp/lex.yy.o
	g++ tmp/lex.yy.o tmp/main.o -o recursive-parser
clean:
	rm tmp/*
	rm recursive-parser
	touch placeholder.tmp
