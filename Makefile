build:
	gcc -Wall -std=c99 ./src/*.c -o game -lSDL2 -lSDL2_mixer

run:
	./game

clean:
	rm game