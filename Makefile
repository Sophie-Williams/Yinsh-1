all: main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	  g++ --std=c++11 -O3 -o main main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp 

clean: 
	del -f *.exe

opti: main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	  g++ --std=c++11 -O3 -o dSwnnUeAB main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp 

debug: main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	  g++ --std=c++11 -g -o debug main.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp 

tester: tester.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	  g++ --std=c++11 -g -o tester tester.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
      
gamegen: gamegen.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp
	g++ --std=c++11 -g -o gg gamegen.cpp Player.cpp Game.cpp Move.cpp MicroMove.cpp Utility.cpp

dumb: dumbBot.cpp  Utility.cpp
	  g++ --std=c++11 -g -o dumb dumbBot.cpp  Utility.cpp