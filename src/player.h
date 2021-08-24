#ifndef PLAYER_H
#define PLAYER_H

#include <sstream>

#include "filesystem.h"
#include "keylist.h"

using namespace std;


struct Piano_Player {
    
    File_System data;

    string name;
    string description;
    int start_pointer;
    Piano_KeyList keys;

    bool running = false;
    bool finished = false;
    int time;
    int start_time;
    int total_time;

    bool load();
    void start();
    void stop();
    void step();

    bool save(string file_location="_pianosave", unsigned int file_max_size=32*1024);

};


#endif