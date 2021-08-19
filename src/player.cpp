
#include "player.h"

#include <time.h>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;


void Piano_Player::load() {
    data.reset();

    // File type
    if(data.read_char() != '2') return;

    // Header validation
    if(data.read_char() != 0x79 || data.read_char() != 0xBD || data.read_char() != 0xAC) return;
    // Name & description
    name = data.read_string(data.read_char());
    description = data.read_string(data.read_short());

    start_pointer = data.pointer;
}


void Piano_Player::start() {
    data.reset();
    data.pointer = start_pointer;

    running = true;
    finished = false;
    start_time = clock();
}

void Piano_Player::stop() {
    running = false;
    total_time = clock() - start_time;
}


void Piano_Player::step() {

    // Used to compensate for lossed time from pressing keys.
    unsigned int key_count = 0;

    while(time <= clock() && running) {

        unsigned char action = data.read_char();
        unsigned char action_type = action >> 4;
        unsigned char action_val = action & 0x0F;


        // Normal action
        if(action_type <= 0x9) {
            switch(action_type) {
                case 0x0: break; // ACTION: NULL
                case 0x1: { // ACTION: PLAY KEYS
                    if(debug) {
                        cout << "Action PLAY KEYS - count: " << (int)action_val << " - keys: " << data.read_string(action_val) << endl;
                        data.pointer -= action_val;
                    }
                    // Press keys
                    key_count += action_val;
                    for(unsigned char i=action_val; i > 0; i--) {
                        keys.press_key(data.read_char());
                    }
                    break; }
                case 0x2: { // ACTION: DELAY
                    // Get delay
                    unsigned int val;
                    switch(action_val) {
                        case 0x0: val = data.read_char(); break;
                        case 0x1: val = data.read_short(); break;
                        case 0x2: val = data.read_int(); break;
                    }

                    // Add delay to time
                    time = clock() + val - key_count * keys.key_time;
                    // Don't subtract key delay if it will get it stuck forever.
                    if(time <= clock()) time = clock() + val;
                    // This is the easiest way I found to fix it.
                    // Too lazy to fix it in other ways so just deal with it.

                    if(debug) {
                        cout << "Action DELAY - delay: " << val << " - " << key_count << "*" << keys.key_time << endl;
                    }
                    break; }
            }
        } else { // Meta action
            action_type = action;
            switch(action_type) {
                case 0xFF: { // META ACTION: END SONG
                    if(debug) {
                        cout << "Meta Action END SONG" << endl;
                    }
                    finished = true;
                    stop();
                    break; }
                case 0xFA: { // META ACTION: PRINT
                    if(debug) {
                        cout << "Meta Action PRINT - string: ";
                    }
                    cout << data.read_string(data.read_char()) << endl;
                    break; }
            }
        }


        // Auto stop playing at end of data
        if(data.eof) {
            finished = true;
            stop();
        }

    }

}






bool Piano_Player::save(string file_location, unsigned int file_max_size) {
    string file_string = data.data_string();

    if((unsigned int)file_string.length() > file_max_size) {
        return false;
    }


    ofstream file(file_location + ".txt");
    file << file_string;
    file.close();
    return true;
}



