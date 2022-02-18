all: formatter NaivePLW_Parallel

formatter: formatter.cpp
	g++ -o formatter formatter.cpp

indexedSet: indexedSet.cpp 
	g++ -Wall -c indexedSet.cpp -o indexedSet.o 

errorHandler: errorHandler.cpp 
	g++ -Wall -c errorHandler.cpp -o errorHandler.o

channel: channel.cpp
	g++ -Wall -c channel.cpp -o channel.o

NaivePLW_Parallel: NaivePLW.cpp channel errorHandler indexedSet
	g++ -Wall -o NaivePLW NaivePLW.cpp channel.o errorHandler.o indexedSet.o -pthread

clean:
	rm -rf formatter NaivePLW *.txt *.o
