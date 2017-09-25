all: cross_socket.o cross_library.o game.o main.o
	g++ -o new_puyo_c cross_socket.o cross_library.o game.o main.o

cross_library.o: cross_library.cpp cross_library.h
	g++ -c cross_library.cpp -std=c++11

cross_socket.o: cross_socket.cpp cross_socket.h
	g++ -c cross_socket.cpp -std=c++11

game.o: game.cpp game.h board.h puyo.h player_information.h WELLRNG512Gen.h cross_library.h common.h
	g++ -c game.cpp -std=c++11

main.o: main.cpp game.h board.h puyo.h player_information.h WELLRNG512Gen.h cross_library.h common.h
	g++ -c main.cpp -std=c++11

# tests
test: welltest cltest sockettest

welltest: WELLRNG512GenTest.cpp WELLRNG512Gen.h
	g++ -c WELLRNG512GenTest.cpp -std=c++11
	g++ -o well512test WELLRNG512GenTest.o

cltest: cross_library_test.cpp cross_library.o
	g++ -c cross_library_test.cpp -std=c++11
	g++ -o cltest cross_library_test.o cross_library.o -lncurses

sockettest: cross_socket_test.cpp cross_socket.o
	g++ -c cross_socket_test.cpp -std=c++11
	g++ -o sockettest cross_socket_test.o cross_socket.o

cleantest:
	rm -f well512test
	rm -f cltest
	rm -f sockettest
	rm -f *.o

clean: cleantest
	rm -f *.o
	rm -f new_puyo_c
