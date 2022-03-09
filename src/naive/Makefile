all: formatter NaivePLW


chain: chain.cpp
	g++ -Wall -c chain.cpp -o chain.o

channel: channel.cpp
	g++ -Wall -c channel.cpp -o channel.o

checkers: checkers.cpp
	g++ -Wall -c checkers.cpp -o checkers.o

errorHandler: errorHandler.cpp
	g++ -Wall -c errorHandler.cpp -o errorHandler.o

formatter: formatter.cpp
	g++ -o formatter formatter.cpp

indexedSet: indexedSet.cpp 
	g++ -Wall -c indexedSet.cpp -o indexedSet.o

linalg: linalg.cpp 
	g++ -Wall -c linalg.cpp -o linalg.o

NaivePLW: NaivePLW.cpp chain channel checkers errorHandler indexedSet linalg
	g++ -Wall -o NaivePLW NaivePLW.cpp chain.o channel.o checkers.o errorHandler.o indexedSet.o linalg.o -pthread

clean:
	rm -rf formatter NaivePLW *.txt *.o
