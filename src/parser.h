#ifndef PARSER_H
#define PARSER_H

#include <sstream>

#include "filesystem.h"

using namespace std;


struct Piano_Parser {

    File_System data;

    bool parse_file(string file_location);
    bool parse_binary();
    bool parse_song(string file_string);
    bool parse_midi(File_System midi);

    void addDelay(unsigned int delay=0);

    void compress();

};


#endif