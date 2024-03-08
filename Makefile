all: fw.o
	gcc -o all fw.o
	
fw.o: fw.c
	gcc -Wall -g -c -o fw.o fw.c
	

test: all
	./fw < test
	
		

clean:
	rm *.o all
