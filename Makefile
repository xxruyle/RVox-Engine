debug: 
	g++ --std=c++11 -Wall -g  -I ./include -L ./lib -o bin/main src/*.cpp src/*.c -lopengl32 -lglfw3dll  -lgdi32 -lassimp-vc143-mtd -lassimp
run: 
	./bin/main