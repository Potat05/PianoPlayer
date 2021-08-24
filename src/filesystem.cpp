#include "filesystem.h"

#include <sstream>
#include <fstream>
#include <vector>

#include <iostream>
#include <windows.h>


using namespace std;



bool File_System::load_file(string file_location) {

    ifstream file(file_location); // Load file
    
    if(!file.good() || !file.is_open()) return false; // If file not found or failed to open
    
    // Get file size
    file.seekg(0, ios::end);
    int length = file.tellg();
    file.seekg(0, ios::beg);

    if(length <= 0) return false; // Error loading file

    // Name
    name = file_location;

    // Load file into data
    data.reserve(length);
    copy(istreambuf_iterator<char>(file), istreambuf_iterator<char>(), back_inserter(data));
    reset();

    return true;
}

bool File_System::save_file(string file_location, unsigned int max_size) {
    if(data.size() > max_size) return false; // Just incase if there's a error so we don't save a massive file

    // Open file
    ofstream file;
    file.open(file_location);

    if(!file.is_open() || !file) return false; // If file fails to open

    // Save data onto file
    string str = "";
    for(unsigned int i=0; i < data.size(); i++) str += data[i];
    file << str;
    file.close();

    return true;
}

void File_System::clear() {
    data.clear();
    reset();
}



void File_System::reset() {
    pointer = 0;
    eof = false;
    ovr = false;   
}








unsigned char File_System::read_char() {
    if(eof) return 0; // If end of file don't read junk bytes
    if(pointer+1 >= data.size()) eof = true; // Check if pointer is going to be at end of file
    
    return data[pointer++];
}

unsigned char * File_System::read_chars(unsigned int count) {
    unsigned char read[count];
    for(unsigned int i=0; i < count; i++) {
        read[i] = read_char();
    }
    return read;
}

unsigned short File_System::read_short() {
    if(eof) return 0;
    // Combine 2 chars
    return read_char()*256 + read_char()*1;
}

unsigned int File_System::read_int() {
    if(eof) return 0;
    // Combine 2 shorts
    return read_short()*65536 + read_short()*1;
}

string File_System::read_string(unsigned int bytes) {
    string val = "";
    for(unsigned int i=0; i < bytes && !eof; i++) {
        val += read_char();
    }
    return val;
}

string File_System::data_string() {
    string str = "";
    for(unsigned int i=0; i < data.size(); i++) {
        str += data[i];
    }
    return str;
}






void File_System::write_char(unsigned char val) {
    if(pointer+1 >= data.size()) { // Add the val to the end of the data if it's at the data limit
        data.push_back(val);
        ovr = true; // And update this flag accordingly
    } else {
        data[pointer] = val;
    }
    if(inc) pointer++;
    return;
}

void File_System::write_chars(unsigned char vals[], int size=0) {
    for(int i=0; i < size; i++) {
        write_char(vals[i]);
    }
}

void File_System::write_short(unsigned short val) {
    write_char(val >> 8); // Just split the value into 2 bytes then write those
    write_char(val);
}

void File_System::write_int(unsigned int val) {
    write_short(val >> 16); // Just split the value into 2 shorts then write those
    write_short(val);
}

void File_System::write_string(string val) {
    for(unsigned int i=0; i < val.length(); i++) {
        write_char(val[i]);
    }
}



unsigned char File_System::pop() {
    unsigned char val = data[data.size()-1];
    data.pop_back();
    return val;
}




unsigned long long int File_System::read_vlv() {
    bool loop = true;
    unsigned long long int value = 0;
    unsigned long long int base = 1;

    // If final bit in byte isn't set don't goto next byte
    while(loop) {
        unsigned char read = read_char();

        if(read & 128) {
            read -= 128;
        } else {
            loop = false;
        }
        value += read * base;
        base *= 128;
    }

    return value;
}


unsigned long long int File_System::read_bytes(unsigned char count) {
    unsigned long long int value = 0;
    unsigned long long int base = 1;

    for(unsigned char i=0; i < count; i++) {
        unsigned char read = read_char();

        value += read * base;
        base *= 256;
    }

    return value;
}