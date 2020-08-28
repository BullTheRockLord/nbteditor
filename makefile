nbteditor : tag.o tagreader.o display.o main.o
	g++ -o nbteditor tag.o display.cpp tagreader.o main.o -g

tag.o : tag.cpp tag.h output_helper.cpp
	g++ -c tag.cpp -g

tagreader.o : tagreader.cpp tagreader.h
	g++ -c tagreader.cpp -g

display.o : display.cpp
	g++ -c display.cpp

main.o : main.cpp
	g++ -c main.cpp -g

clean :
	rm nbteditor *.o
