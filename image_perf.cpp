
#include "packet_sender/sleep.h"
#include <opencv2/opencv.hpp>
#include "display_manager/display_manager.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h> // usleep 사용

using namespace std;

void test_itp(DisplayManager * display, unsigned char * rgb_data){
	display->display_itp(rgb_data, false);
}

void test_itp_blur(DisplayManager * display, unsigned char * rgb_data){
	display->display_itp(rgb_data, true);
	display->free_image();
}

void test_center(DisplayManager * display, unsigned char * rgb_data){
	display->display(rgb_data, false);
}

void test_center_blur(DisplayManager * display, unsigned char * rgb_data){
	display->display(rgb_data, true);
	display->free_image();
}

void test_mean(DisplayManager * display, unsigned char * rgb_data){
	display->display_mean(rgb_data, false);
}

void test_mean_blur(DisplayManager * display, unsigned char * rgb_data){
	display->display_mean(rgb_data, true);
	display->free_image();
}

int main() {
    int mode = 0;
    std::string image_path = "red_wave.jpg"; 

    cv::Mat frame = cv::imread(image_path);
    if (frame.empty()) {
        std::cerr << "Error: Could not open image file." << std::endl;
        return -1;
    }

    const char func[6][16] = {"itp", "itp-blur", "mean", "mean-blur", "center", "center-blur"};
    const char *ip = "192.168.50.72"; 
    DisplayManager * display= new DisplayManager(frame.rows, frame.cols, 90, 270, 10, 0, "192.168.50.72");
    E131Sender *sender = new E131Sender(ip);
    
    while (true) {
        unsigned char *rgb_data = frame.data;
        switch(mode/20){
            case 0:
		test_mean(display, rgb_data);
                break;
            case 1:
		test_mean_blur(display, rgb_data);
                break;
            case 2:
		test_itp(display, rgb_data);
                break;
            case 3:
		test_itp_blur(display, rgb_data);
                break;
            case 4:
		test_center(display, rgb_data);
                break;
            case 5:
		test_center_blur(display, rgb_data);
                break;
            default:
                break;
        }
	mode++;
	if(mode >= 120) break;
    }
    cv::destroyAllWindows();
    delete sender;

    return 0;
}
