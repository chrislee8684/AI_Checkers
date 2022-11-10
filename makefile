checkers.exe: main.o initialize.o backend.o
	g++ -std=c++11 -o checkers.exe main.o initialize.o backend.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

initialize.o: Initialize.cpp Initialize.h
	g++ -std=c++11 -c Initialize.cpp

backend.o: Backend.cpp Backend.h
	g++ -std=c++11 -c Backend.cpp

debug:
	g++ -std=c++11 -g -o checkersDebug.exe main.cpp Initialize.cpp Backend.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
