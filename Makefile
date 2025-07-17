test: test.cpp image_processor/*.* packet_sender/*.*
	g++ -o test test.cpp image_processor/image_processor.cpp packet_sender/e131_sender.cpp -le131
mac: test_txt.cpp image_processor/*.* packet_sender/*.*
	g++ -std=c++11 -o test test.cpp image_processor/image_processor.cpp packet_sender/e131_sender.cpp -I/opt/homebrew/include/opencv4 -L/opt/homebrew/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -le131

youtube_stream: youtube_stream.cpp image_processor/*.* packet_sender/*.*
	g++ -std=c++17 -o youtube_stream \
    youtube_stream.cpp \
    packet_sender/e131_sender.cpp \
    image_processor/image_processor.cpp \
    -L/path/to/e131/lib -le131 \
    `pkg-config --cflags --libs opencv4 sdl2` \
    -ljsoncpp


video: video_test.cpp
	g++ -std=c++17 -o video_test video_test.cpp packet_sender/e131_sender.cpp image_processor/image_processor.cpp  -L/path/to/e131/lib -le131 $(pkg-config --cflags --libs opencv4 sdl2) -ljsoncpp


\image: image_test.cpp image_processor/*.* packet_sender/*.*
	g++ -std=c++17 -o image_test image_test.cpp packet_sender/e131_sender.cpp image_processor/image_processor.cpp -L/path/to/e131/lib -le131 $(pkg-config --cflags --libs opencv4) -ljsoncpp
