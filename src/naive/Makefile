.PHONY: all formatter NaivePLW NaivePLW_dbg analyser clean

CXX = g++
CXXFLAGS  = -Wall
PTHREADFLAG = -pthread
UTIL_FILES = chain channel checkers data distSelector errorHandler indexedSet linalg 

all: NaivePLW analyser

$(UTIL_FILES).o:
	$(CXX) ${CXXFLAGS} -c $(UTIL_FILES).c -o $(UTIL_FILES).o

formatter: formatter.cpp
	${CXX} -o formatter formatter.cpp

NaivePLW: NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o
	${CXX} ${CXXFLAGS} -o NaivePLW NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o ${PTHREADFLAG}

NaivePLW_dbg: NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o
	${CXX} -g ${CXXFLAGS} -o NaivePLW_dbg NaivePLW.cpp chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o ${PTHREADFLAG}

converter: converter.cpp data.o checkers.o errorHandler.o
	${CXX} ${CXXFLAGS} -o converter converter.cpp data.o checkers.o errorHandler.o

analyser: analyser.cpp data.o checkers.o errorHandler.o linalg.o
	${CXX} ${CXXFLAGS} -o analyser analyser.cpp data.o checkers.o errorHandler.o linalg.o

clean:
	rm -rf formatter NaivePLW *.txt *.o *.out NaivePLW_dbg converter analyser
