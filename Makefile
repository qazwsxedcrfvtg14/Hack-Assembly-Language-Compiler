all: out/example.asm out/snake.asm

out/example: example/main.cpp
	g++ -std=c++14 example/main.cpp -I compiler -o out/example

out/example.asm: out/example
	out/example >out/example.asm

out/snake: snake/main.cpp
	g++ -std=c++14 snake/main.cpp -I compiler -o out/snake

out/snake.asm: out/snake
	out/snake >out/snake.asm

clean :
	rm out/*
