build:
	g++ src/*.cpp -lglut -lGL -o space-invaders
clean:
	rm space-invaders