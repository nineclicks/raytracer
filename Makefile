all: tracer

tracer: tracer.cpp scene.cpp vector.cpp object.cpp fimage.cpp parser.cpp
	g++ -pthread tracer.cpp scene.cpp vector.cpp object.cpp fimage.cpp parser.cpp -o tracer -Wall -Wextra
	
clean:
	rm -f tracer

