 all: main.cpp Player.cpp Game.cpp Move.cpp
	  g++ --std=c++11 -o main main.cpp Player.cpp Game.cpp Move.cpp

clean: 
	rm -f main