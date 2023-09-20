#include <iostream>
#include "message_transceiver.h"
using namespace std;

int main(int argc, char** argv){
    if(argc != 2){
        cout << "wrong number of input arguements\n";
        return 1;
    }
    
    cout << "[starting node: " << argv[1] << "](hit enter to send message)\n";

    MessageTransceiver transceiver(argv[1]);
    
    transceiver.run();

    return 0;
}