#include <cstdint>
#include <cstdlib>
#include <e131.h>
#include <iostream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <string>
#include <jsoncpp/json/json.h>
#include <fstream>
#include "../packet_sender/e131_sender.h"
#include "../image_processor/image_processor.h"


using namespace std;

#define DEFAULT_PATH "/opt/sphere/sphere_spec.json"

class DisplayManager{
    public:
        DisplayManager(int imheight, int imwidth, int sratio, int eratio, int tpad, int dpad, const char* ip, int port=E131_DEFAULT_PORT, string path_to_json=DEFAULT_PATH){
            this->json_path = path_to_json;
            this->json.open(this->json_path, ifstream::binary);
            if(!json){
                cout << "failed to read json (" << this->json_path << ")" << endl;
            }
            string errors;
            if (!Json::parseFromStream(this->json_reader, this->json, &this->json_root, &errors)) {
                cerr << "JSON parse failed: " << errors << std::endl;
                exit(EXIT_FAILURE);
            }
            height = this->json_root["height"].asInt();
            int led_size = height;
            if(height != led_size){
                cerr << "Size of led array should be same with height. (height : " << height << ", led_size : " << led_size << ")"<<endl;
               // exit(EXIT_FAILURE);
            }

            this->leds = new int[led_size];
            for(int i = 0; i < led_size; i++){
                this->leds[i] = this->json_root["led"][i].asInt();
                cout << this->leds[i] << " ";
            }
            
            int alias_size = height;
            if(height != alias_size){
                cerr << "Size of led array should be same with height. (height : " << height << ", led_size : " << alias_size << ")"<<endl;
                exit(EXIT_FAILURE);
            }

            this->alias = new int[alias_size];
            for(int i = 0; i < alias_size; i++){
                this->alias[i] = this->json_root["alias"][i].asInt();
            }
            sender = new E131Sender(ip, port);
            processor = new ImageProcessor(imheight, imwidth, tpad, sratio, eratio, dpad, leds, this->height, this->alias);
            processor->calc_row();
        }
        void display(uint32_t* pixels, bool blur){
            if(blur) this->processor->blur();
            this->processor->mask(pixels);
            this->processor->rotate();
            sender->send(this->processor->get_processed_image(), this->processor->get_processed_image_size());
            sender->next();
        }
        
        void display(unsigned char* pixels, bool blur){
            this->processor->image = pixels;
            if(blur) {
                this->processor->blur();
            }
            this->processor->mask(this->processor->image);
            this->processor->rotate();
            sender->send(this->processor->get_processed_image(), this->processor->get_processed_image_size());
            sender->next();
        }
        void display_itp(unsigned char* pixels, bool blur){
            this->processor->image = pixels;
            if(blur) {
                this->processor->blur();
            }
            this->processor->mask_itp(this->processor->image);
            this->processor->rotate();
            sender->send(this->processor->get_processed_image(), this->processor->get_processed_image_size());
            sender->next();
        }
        void display_mean(unsigned char* pixels, bool blur){
            this->processor->image = pixels;
            if(blur) {
                this->processor->blur();
            }
            this->processor->mask_mean(this->processor->image);
            this->processor->rotate();
            sender->send(this->processor->get_processed_image(), this->processor->get_processed_image_size());
            sender->next();
        }
        void free_image(){
            processor->free_image();
        }
    private:
        string json_path;
        Json::Value json_root;
        Json::CharReaderBuilder json_reader;
        ifstream json;
        int height;
        int* leds;
        int* alias;
        E131Sender * sender;
        ImageProcessor * processor;
};
