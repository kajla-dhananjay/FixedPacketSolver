all: formatter NaivePLW_Parallel

formatter: formatter.cpp
	g++ -o formatter formatter.cpp

NaivePLW_Serial: NaivePLW.cpp
	g++ -o NaivePLW NaivePLW.cpp

NaivePLW_Parallel: NaivePLW.cpp
	g++ -Wall -o NaivePLW NaivePLW.cpp -DPARALLEL -pthread -DDEBUG

clean:
	rm -rf formatter NaivePLW *.txt
