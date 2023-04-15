debug: 
	g++ --std=c++11 -Wall -g -I ./include -L ./lib -o bin/main src/*.cpp src/*.c -lopengl32 -lglfw3 -lgdi32  

run: 
	./bin/main