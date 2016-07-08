
all:
	
	

# tests
test: welltest cltest

welltest: WELLRNG512GenTest.cpp WELLRNG512Gen.h
	g++ -c WELLRNG512GenTest.cpp -std=c++11
	g++ -o well512test WELLRNG512GenTest.o

cltest: cross_library_test.cpp cross_library.h
	g++ -c cross_library_test.cpp -std=c++11
	g++ -o cltest cross_library_test.o -lncurses

cleantest:
	rm -f well512test
	rm -f cltest

clean: cleantest
	rm -f *.o
