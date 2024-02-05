all:
	g++ -O3 main.cpp -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2 -o main