
all:
	
	

# tests
test: welltest

welltest: WELLRNG512GenTest.cpp WELLRNG512Gen.h
	g++ -c WELLRNG512GenTest.cpp -std=c++11
	g++ -o well512test WELLRNG512GenTest.o

cleantest:
	rm -f well512test

clean: cleantest
	rm -f *.o
