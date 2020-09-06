nbteditor : tag.h tagreader.o display.o main.o
	g++ -o nbteditor display.o tagreader.o main.o -g

tagreader.o : tagreader.cpp tagreader.h
	g++ -c tagreader.cpp -g

display.o : display.cpp
	g++ -c display.cpp -g

main.o : main.cpp
	g++ -c main.cpp -g

clean :
	rm nbteditor *.o
