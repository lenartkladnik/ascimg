g++ ascimg.cpp -o ascimg $(pkg-config --cflags opencv4) -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui --std=c++20
