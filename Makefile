all: compile link
	
compile:
	g++ -c main.cpp -IC:\SFML-2.5.1\include

link:
	g++ main.o -o main -L C:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system