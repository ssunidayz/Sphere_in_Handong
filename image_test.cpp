
#include "packet_sender/sleep.h"
#include <opencv2/opencv.hpp>
#include "display_manager/display_manager.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h> // usleep 사용

using namespace std;

int main() {
    int mode = 0;
    std::string image_path = "image/red_wave.jpg"; 

    cv::Mat frame = cv::imread(image_path);
    if (frame.empty()) {
        std::cerr << "Error: Could not open image file." << std::endl;
        return -1;
    }

    int rows[54] = {
        104, 126, 141, 157, 172, 187, 199, 211, 224, 235, 246, 255, 263, 272, 279, 286, 293, 299,
        306, 311, 316, 320, 326, 331, 335, 337, 342, 345, 347, 351, 353, 355, 357, 356, 358, 359,
        359, 359, 359, 358, 358, 356, 355, 353, 351, 348, 345, 342, 338, 334, 330, 325, 319, 314
    };

    const char func[6][16] = {"itp", "itp-blur", "mean", "mean-blur", "center", "center-blur"};
    const char *ip = "192.168.50.72"; 
    DisplayManager * display= new DisplayManager(frame.rows, frame.cols, 90, 270, 5, 0, "192.168.50.72");
    E131Sender *sender = new E131Sender(ip);
    
    while (true) {
        unsigned char *rgb_data = frame.data;
        auto start = chrono::high_resolution_clock::now();
        switch(mode){
            case 0:
                display->display_itp(rgb_data, false);
                break;
            case 1:
                display->display_itp(rgb_data, true);
                display->free_image();
                break;
            case 2:
                display->display_mean(rgb_data, false);
                break;
            case 3:
                display->display_mean(rgb_data, true);
                display->free_image();
                break;
            case 4:
                display->display(rgb_data, false);
                break;
            case 5:
                display->display(rgb_data, true);
                display->free_image();
                break;
            default:
                break;
        }
        auto end = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<std::chrono::microseconds>(end-start);
        cout << "1time" << duration.count() << " usec" << endl;

        
        // OpenCV 화면 출력
        cv::imshow("Static Image Display", frame);
        int key = cv::waitKey(500);

        // ESC 키(27) 누르면 종료
        if (key == 'q') break;
        else if(key == 'a') {
            mode = mode-1 < 0 ? 5 : mode-1;
            cout << "mode : " << func[mode] << endl;
        }
        else if(key == 'd'){
            mode = (mode+1)%6;
            cout << "mode : " << func[mode] << endl;
        }

    }
    cv::destroyAllWindows();
    delete sender;

    return 0;
}
