all: formatter NaivePLW_Serial

formatter: formatter.cpp
	g++ -o formatter formatter.cpp

NaivePLW_Serial: NaivePLW.cpp
	g++ -o NaivePLW NaivePLW.cpp

NaivePLW_Parallel: NaivePLW.cpp
	g++ -DPARALLEL -o NaivePLW NaivePLW.cpp

clean:
	rm -rf formatter NaivePLW *.txt
