.PHONY: all clean install uninstall
all: test
clean:
	rm -rf leaks *.o
errors.o: error_class.cpp
	g++ -c -Weffc++ -o errors.o error_class.cpp
test.o: test.cpp
	g++ -c -Weffc++ -o test.o test.cpp
test: test.o errors.o
	g++ test.o errors.o -o test
install: 
	install ./game /usr/local/bin
uninstall:
	rm -rf /usr/local/bin/test
