all: main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	  g++ --std=c++11 -o main main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp 

clean: 
	del -f *.exe

debug: main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	  g++ --std=c++11 -g -o main main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp 
