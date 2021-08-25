
#include "parser.h"

#include <sstream>
#include <iostream>
#include <windows.h>
#include <fstream>

#include "globals.h"

using namespace std;



bool Piano_Parser::parse_file(string file_location) {
    string file_extension = file_location.substr(file_location.rfind("."), 999);

    data.load_file(file_location);

    data.reset();

    if(file_extension == ".txt") {

        switch(data.read_char()) {
            case '1': {
                ifstream file(file_location);
                string file_string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
                return parse_song(file_string);
                break; }
            case '2':
                return parse_binary();
                break;
        }

    } else if(file_extension == ".mid" || file_extension == ".midi") {
        return parse_midi(data);
    }

    return false;
}









void Piano_Parser::addDelay(unsigned int delay) {
    if(delay <= 255) {
        data.write_char(0x20);
        data.write_char(delay);
        return;
    }
    if(delay <= 65535) {
        data.write_char(0x21);
        data.write_short(delay);
        return;
    }
    data.write_char(0x22);
    data.write_int(delay);
    return;
}




// Basically just combine keys and delays.
void Piano_Parser::compress() {
    // Compressed file
    File_System comp;
    data.reset();


    // Type & validation
    comp.write_char('2');
    comp.write_int(0x79BDAC3F);
    data.read_chars(5);

    // Name & description
    unsigned char name_length = data.read_char();
    comp.write_char(name_length);
    comp.write_string(data.read_string(name_length));
    unsigned short description_length = data.read_short();
    comp.write_short(description_length);
    comp.write_string(data.read_string(description_length));



    // Actions
    bool reading_keys = false;
    unsigned char keys_count = 0;
    unsigned char keys[15];
    bool reading_delays = false;
    unsigned int delay = 0;
    while(!data.eof) {

        unsigned char action = data.read_char();
        unsigned char action_type = action >> 4;
        unsigned char action_val = action & 0x0F;

        // Add keys
        if(reading_keys && (action_type != 0x1 || keys_count >= 16)) {
            comp.write_char((0x01 << 4) | keys_count);
            for(unsigned char i=0; i < keys_count; i++) {
                comp.write_char(keys[i]);
            }
            keys_count = 0;
            reading_keys = false;
        }
        // Add delays
        if(reading_delays && action_type != 0x2) {
            if(delay <= 255) {
                comp.write_char(0x20);
                comp.write_char(delay);
            } else if(delay <= 65535) {
                comp.write_char(0x21);
                comp.write_short(delay);
            } else {
                comp.write_char(0x22);
                comp.write_int(delay);
            }
            delay = 0;
            reading_delays = false;
        }


        // ACTIONS
        if(action_type <= 0x9) {
            switch(action_type) {
                case 0x0: break; // NULL
                case 0x1: { // KEYS
                    reading_keys = true;
                    for(unsigned char i=action_val; i > 0; i--) {
                        keys[keys_count] = data.read_char();
                        keys_count++;
                    }
                    break; }
                case 0x2: { // DELAY
                    reading_delays = true;
                    switch(action_val) {
                        case 0x0:
                            delay += data.read_char();
                            break;
                        case 0x1:
                            delay += data.read_short();
                            break;
                        case 0x2:
                            delay += data.read_int();
                            break;
                    }
                    break; }
            }
        } else { // Meta action
            action_type = action;
            switch(action_type) {
                case 0xFF: // END SONG
                    comp.write_char(0xFF);
                    break;
                case 0xFA: { // STRING
                    unsigned char byte = data.read_char();
                    comp.write_char(byte);
                    comp.write_string(data.read_string(byte));
                    break; }
            }
        }

    }


    // Fix a weird bug where it wont play the last key unless i got another action
    comp.write_char(0x20);
    comp.write_char(0x00);


    data = comp;
}