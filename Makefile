all: face_detection

face_detection: face_detection.o
	g++ face_detection.o -o face_detection `pkg-config --libs opencv` 

face_detection.o: face_detection.cpp
	g++ -c face_detection.cpp `pkg-config --cflags opencv` 
	@echo 'Finished building target: $@'

clean:
	rm -rf *o face_detection
