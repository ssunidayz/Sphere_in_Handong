#include "packet_sender/e131_sender.h"
#include "image_processor/image_processor.h"
#include "packet_sender/sleep.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define WIDTH 525
#define HEIGHT 675
#define CHANNELS 3

int main() {
    unsigned char data[WIDTH * HEIGHT * CHANNELS];
    std::ifstream file("go.txt2", std::ios_base::in); // 파일 이름을 적절히 변경하세요

    if (!file) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    std::string line;
    int index = 0;

    while (std::getline(file, line)) {
        // UTF-8 BOM 제거
        if (index == 0 && line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
            line = line.substr(3);
        }
        
        std::stringstream ss(line);
        int value;
        while (ss >> value) {
            if (index >= WIDTH * HEIGHT * CHANNELS) {
                std::cerr << "파일 데이터가 예상보다 많습니다." << std::endl;
                return 1;
            }
            data[index++] = static_cast<unsigned char>(value);
        }
    }


    if (index < WIDTH * HEIGHT * CHANNELS) {
        std::cerr << "파일 데이터가 부족합니다. 읽은 개수: " << index << std::endl;
        return 1;
    }
    int rows[54]={
        104,126,141,157,172,187,199,211,224,235,246,255,263,272,279,286,293,299,306,311,316,320,326,331,335,337,342,345,347,351,353,355,357,356,358,359,359,359,359,358,358,356,355,353,351,348,345,342,338,334,330,325,319,314
    };

    const char * ip= "192.168.50.72";

    ImageProcessor * image = new ImageProcessor(HEIGHT, WIDTH, 0, 90, 270, 0, rows, 54);
    E131Sender * sender = new E131Sender(ip);

    image->mask(data);
    image->rotate();
    
    return 0;
}

