
#include "parser.h"

#include <sstream>
#include <iostream>


using namespace std;



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