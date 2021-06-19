
#include "parser.h"

#include <sstream>
#include <iostream>
#include <windows.h>
#include <fstream>

using namespace std;


bool Piano_Parser::parse_binary() {
    data.reset();
    return true;
}

bool Piano_Parser::parse_song(string file_string) {
    data.clear();

    // Initial values
    data.write_char('2');
    unsigned char validation[3] = {0x79, 0xBD, 0xAC};
    data.write_chars(validation, 3);


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


    return true;
}

bool Piano_Parser::parse_midi() {
    data.reset();
    return false;
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
        return parse_midi();
    }

    return false;
}








void Piano_Parser::addKeys(char keys[]) {
    int count = *(&keys + 1) - keys;
    if(count == 0) return;
    // if(count >= 16) {
    //     char split1[15];
    //     char split2[count-15];
    //     for(int i=0; i < count; i++) {
    //         if(i <= 15) split1[i] = keys[i];
    //         else split2[i-15] = keys[i];
    //     }
    //     addKeys(split1);
    //     addKeys(split2);
    //     return;
    // }

    // data.write_char(0x1 + count);
    // for(int i=0; i < count; i++) {
    //     data.write_char(keys[i]);
    // }

    for(int i=0; i < count; i++) {
        if(i % 15 == 0) {
            data.write_char(0x1 + min(16, count-i));
        }
        data.write_char(keys[i]);
    }

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

// void Piano_Parser::compress() {
//     // Compressed file
//     File_System comp;
//     data.reset();

//     // Type & validation
//     comp.write_chars(data.read_chars(4), 4);
//     // Name & description
//     unsigned char name_length = data.read_char();
//     comp.write_char(name_length);
//     comp.write_string(comp.read_string(name_length));
//     unsigned short description_length = data.read_short();
//     comp.write_short(description_length);
//     comp.write_string(comp.read_string(description_length));

//     // Actions
//     bool reading_keys = false;
//     unsigned char keys_count = 0;
//     unsigned char keys[15];
//     bool reading_delays = false;
//     unsigned int delay = 0;
//     while(!data.eof) {

//     }


//     data = comp;
// }