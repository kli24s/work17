all: control.o write.o
	gcc -o control control.o
	gcc -o write write.o

control.o: control.c
	gcc -c control.c

write.o: write.c
	gcc -c write.c

run:
	./control
	./write

clean:
	rm *.o
	rm control
	rm write
	rm telephone
