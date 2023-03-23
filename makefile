CC = g++ -std=c++11

mizzo: mizzo.cpp buffer.o producers.o consumers.o io.o
	${CC} -g -o mizzo mizzo.cpp buffer.o producers.o consumers.o io.o -lpthread -lrt
	rm *.o
buffer.o: buffer.h buffer.cpp
	${CC} -c buffer.cpp
producers.o: producers.h producers.cpp
	${CC} -c producers.cpp
consumers.o: consumers.h consumers.cpp
	${CC} -c consumers.cpp
io.o: io.h io.c
	${CC} -c io.c
clean:
	rm mizzo