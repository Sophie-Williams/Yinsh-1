all: main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	  g++ --std=c++11 -o main main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp 

clean: 
	rm -f main