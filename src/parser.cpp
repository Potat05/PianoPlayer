
#include "parser.h"

#include <sstream>
#include <iostream>
#include <windows.h>
#include <fstream>

#include "globals.h"

using namespace std;


bool Piano_Parser::parse_binary() {
    data.reset();
    return true;
}

bool Piano_Parser::parse_song(string file_string) {
    data.clear();

    // Initial values
    data.write_char('2');
    data.write_int(0x79BDAC3F);


    // Split the data on each new line
    string data_split[5];
    int j=0;
    for(int i=0; i < (int)file_string.length(); i++) {
        if(j >= 5) {
            cout << "Song file format is incorrect!" << endl;
            return false;
        }
        if(file_string[i] == '\n') j++;
        else data_split[j] += file_string[i];
    }

    // Name & description
    data.write_char(data_split[1].length());
    data.write_string(data_split[1]);
    data.write_short(data_split[2].length());
    data.write_string(data_split[2]);

    // Delays
    unsigned int delays[4] = {200, 300, 400, 100};
    stringstream delays_split(data_split[3]); // Split delays
    for(int i=0; delays_split.good(); i++) {
        string string_sub;
        getline(delays_split, string_sub, ' ');
        stringstream ss(string_sub);
        unsigned int x = 0;
        ss >> x;
        delays[i] = x;
    }


    
    // Convert text to the song data
    string song = data_split[4];
    for(int i=0; i < (int)song.length(); i++) {
        switch(song[i]) {
            case ' ':
                addDelay(delays[1]);
                break;
            case '|':
                addDelay(delays[2]);
                break;
            case '[':
                i++;
                while(song[i] != ']') {
                    if(song[i] == ' ') {
                        addDelay(delays[3]);
                    } else {
                        data.write_char(0x11);
                        data.write_char(song[i]);
                    }
                    i++;
                }
                if(song[i+1] != ' ' && song[i+1] != '|') {
                    addDelay(delays[0]);
                }
                break;
            case '{': {
                i++;
                string delay_numstr = "";
                while(song[i] != '}') delay_numstr += song[i++];
                stringstream delay_ss(delay_numstr);
                int delay_num = 0;
                delay_ss >> delay_num;
                addDelay(delay_num);
                // i++;
                break; }
            default:
                data.write_char(0x11);
                data.write_char(song[i]);
                if(song[i+1] != ' ' && song[i+1] != '|') {
                    addDelay(delays[0]);
                }
                break;
        }
    }

    compress();


    return true;
}

bool Piano_Parser::parse_midi(File_System midi) {
    midi.reset();

    // MIDI header validation
    if(midi.read_int() != 0x4D546864) {
        if(debug) cout << "MIDI header validation failed!" << endl;
        return false;
    }


    // Initial values
    data.clear();
    data.write_char('2');
    data.write_int(0x79BDAC3F);

    unsigned char song_data_type = 0;






    midi.read_int(); // Header size (unused)
    unsigned short format_type = midi.read_short();
    unsigned short track_count = midi.read_short();

    bool time_div_mode = false;
    unsigned short time_div;
    unsigned char time_div1 = midi.read_char();
    unsigned char time_div2 = midi.read_char();
    if(time_div1 > 128) {
        time_div_mode = true;
        time_div1 -= 128;
    } else {
        time_div = time_div1*256 + time_div2;
    }


    // Load first track


    // Track header validation
    unsigned int header = midi.read_int();
    if(header != 0x4D54726B) {
        if(debug) cout << "MIDI track header validation failed!" << endl;
        return false;
    }

    midi.read_int(); // Track size (unused)
    unsigned int event_counter = 0;
    bool end_of_track = false;
    unsigned char status_byte;
    unsigned char last_status_byte;

    while(!end_of_track) {
        event_counter++;

        if(midi.eof) break;

        unsigned long long int delta_time = midi.read_vlv();

        status_byte = midi.read_char();
        if(status_byte >= 128) {
            last_status_byte = status_byte;
        } else {
            status_byte = last_status_byte;
            midi.pointer--;
        }

        if(status_byte == 0xFF) { // Meta action
            unsigned char meta_type = midi.read_char();
            unsigned long long int meta_length = midi.read_vlv();

            switch(meta_type) {
                case 0x2f: // End of track
                    end_of_track = true;
                    break;
                case 0x08:
                case 0x0A:
                case 0x01: // Text event
                case 0x02: { // Copyright
                    // Set name & description
                    if(song_data_type == 0) {
                        string name = midi.read_string(meta_length);
                        data.write_char((unsigned char)name.length());
                        data.write_string(name);
                        song_data_type = 1;
                    } else if(song_data_type == 1) {
                        string description = midi.read_string(meta_length);
                        data.write_short((unsigned short)description.length());
                        data.write_string(description);
                        song_data_type = 2;
                    }
                    break; }
                case 0x03:
                case 0x04: // Instrument name
                case 0x05: // Lyrics
                case 0x07: // Cue point
                case 0x06: // Marker
                    midi.read_string(meta_length);
                    break;
                case 0x21: // MIDI port
                case 0x59: // Key signature
                case 0x51: // Set tempo
                    midi.read_bytes(meta_length);
                    break;
                case 0x54: // SMPTE offset
                    data.read_bytes(5);
                    break;
                case 0x58: // Time signature
                    midi.read_int();
                    break;
                default:
                    if(debug) cout << "Unknown MIDI meta event! 0x" << hex << (int)meta_type << " at " << dec << midi.pointer << endl;
                    midi.read_bytes(meta_length);
                    break;
            }
        } else { // Regular action

            // If no song name or description found before actions
            if(song_data_type == 0) {
                // Use file name as song name
                string name = "";
                for(int i=midi.name.rfind("\\")+1; i < (int)midi.name.length(); i++) {
                    if(midi.name[i] == '.') break;
                    name += (midi.name[i] == '_' ? ' ' : midi.name[i]);
                }
                data.write_char((unsigned char)name.length());
                data.write_string(name);
                song_data_type = 1;
            }
            if(song_data_type == 1) {
                string description = "Unknown.";
                data.write_short((unsigned short)description.length());
                data.write_string(description);
                song_data_type = 2;
            }

            unsigned char type = status_byte >> 4;
            unsigned char channel = status_byte & 0x0F;

            switch(type) {
                case 0xF: // System exlusive
                    if(debug) cout << "System exclusive event! 0xF at " << midi.pointer << ",skipping. . ." << endl;
                    midi.read_bytes(midi.read_vlv());
                    break;
                case 0xA: // Note aftertouch
                case 0xB: // Controller
                case 0xE: // Pitch blend
                case 0x8: // Note off
                    midi.read_short();
                    break;
                case 0x9: { // Note on
                    unsigned char piano_key = midi.read_char();
                    if(midi.read_char() != 0x50) break;

                    unsigned int time = ((float)delta_time / (float)time_div) * 100;
                    cout << time << endl;
                    time = 200;
                    addDelay(time);

                    data.write_char(0x41);
                    data.write_char(piano_key);
                    break; }
                case 0xC: // Program change
                case 0xD: // Channel aftertouch
                    midi.read_char();
                    break;
                default:
                    if(debug) cout << "Unknown event! 0x" << hex << (int)type << " at " << dec << midi.pointer << ". Stopped reading." << endl;
                    return false;
                    break;
            }
        }

    }





    return true;

}




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