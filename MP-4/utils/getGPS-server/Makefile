
all:
	gcc  -Wl,-rpath /usr/local/lib -L/usr/local/lib -o getGPS-server -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect Fruit.cpp pseudoGPS.cpp main.cpp

clean:
	rm -rf *.o getGPS-server