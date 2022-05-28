.PHONY: all formatter NaivePLW NaivePLW_dbg analyser

CC = g++

CFLAGS = -Wall 
PTHREADFLAG = -pthread

all: formatter NaivePLW analyser

chain.o: chain.cpp
	${CC} ${CFLAGS} -c chain.cpp -o chain.o

channel.o: channel.cpp
	${CC} ${CFLAGS} -c channel.cpp -o channel.o

checkers.o: checkers.cpp
	${CC} ${CFLAGS} -c checkers.cpp -o checkers.o

data.o: data.cpp
	${CC} ${CFLAGS} -c data.cpp -o data.o 

distSelector.o: distSelector.cpp
	${CC} ${CFLAGS} -c distSelector.cpp -o distSelector.o

errorHandler.o: errorHandler.cpp
	${CC} ${CFLAGS} -c errorHandler.cpp -o errorHandler.o

formatter: formatter.cpp
	${CC} -o formatter formatter.cpp

indexedSet.o: indexedSet.cpp 
	${CC} ${CFLAGS} -c indexedSet.cpp -o indexedSet.o

linalg.o: linalg.cpp 
	${CC} ${CFLAGS} -c linalg.cpp -o linalg.o

NaivePLW: NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o
	${CC} ${CFLAGS} -o NaivePLW NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o ${PTHREADFLAG}

NaivePLW_dbg: NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o
	${CC} -g ${CFLAGS} -o NaivePLW_dbg NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o ${PTHREADFLAG}

converter: converter.cpp data.o checkers.o errorHandler.o
	${CC} ${CFLAGS} -o converter converter.cpp data.o checkers.o errorHandler.o

analyser: analyser.cpp data.o checkers.o errorHandler.o linalg.o
	${CC} ${CFLAGS} -o analyser analyser.cpp data.o checkers.o errorHandler.o linalg.o

clean:
	rm -rf formatter NaivePLW *.txt *.o *.out NaivePLW_dbg converter analyser
