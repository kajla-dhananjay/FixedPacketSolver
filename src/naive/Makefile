.PHONY: all clean

CXX = g++
CXXFLAGS  = -Wall
PTHREADFLAG = -pthread
UTIL_FILES = chain channel checkers data distSelector errorHandler indexedSet linalg NaivePLW
UTIL_OBJS = chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o NaivePLW.o

all: plw analyser formatter

$(UTIL_OBJS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

formatter: formatter.cpp
	${CXX} -o formatter formatter.cpp

plw: plw.cpp NaivePLW.o chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o
	${CXX} ${CXXFLAGS} -o plw plw.cpp NaivePLW.o chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o ${PTHREADFLAG}

plw_dbg: plw.cpp NaivePLW.o chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o
	${CXX} ${CXXFLAGS} -o plw_dbg plw.cpp NaivePLW.o chain.o channel.o checkers.o data.o distSelector.o errorHandler.o indexedSet.o linalg.o ${PTHREADFLAG}

converter: converter.cpp data.o checkers.o errorHandler.o
	${CXX} ${CXXFLAGS} -o converter converter.cpp data.o checkers.o errorHandler.o

analyser: analyser.cpp data.o checkers.o errorHandler.o linalg.o
	${CXX} ${CXXFLAGS} -o analyser analyser.cpp data.o checkers.o errorHandler.o linalg.o

clean:
	rm -rf formatter plw *.txt *.o *.out plw_dbg converter analyser