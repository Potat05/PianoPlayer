#ifndef FILESYSTEM_H
#define FILESYSTEM_H


#include <sstream>
#include <vector>


using namespace std;



/*

    We don't care about about speed here as long as it gets the job done it's good!

*/



struct File_System {
    unsigned int pointer = 0; // Byte pointer
    vector<unsigned char> data; // Data as bytes
    string name = "null"; // Name of file

    // Saving & loading file
    bool load_file(string file_location); // Load file into data
    bool save_file(string file_location, unsigned int max_size=32*1024); // Save data into file at location
    void clear(); // Clears data


    void reset(); // Reset some stuff


    // Reading from data
    bool eof = false; // If at end of data
    unsigned char read_char(); // Read char
    unsigned char * read_chars(unsigned int count=0); // Read amount of chars
    unsigned short read_short(); // Read short
    unsigned int read_int(); // Read int
    string read_string(unsigned int bytes=1); // Read string

    string data_string(); // Returns data as string (Would only recommend for smaller files)

    // Writing to data
    bool inc = true; // If to increment pointer on each write
    bool ovr = false; // If data that is being written extending the data array
    void write_char(unsigned char val=0x00); // Write char
    void write_chars(unsigned char vals[], int size); // Write character array
    void write_short(unsigned short val=0x0000); // Write short
    void write_int(unsigned int val=0x00000000); // Write int
    void write_string(string val=""); // Write string

    unsigned char pop(); // Removes last char from data and returns

    // Basically just reads any length number
    // if last bit is set it reads another byte
    // The max is 32 bits but we do 64 bits just incase
    unsigned long long int read_vlv(); // For MIDI files

    // Read amount of bytes
    unsigned long long int read_bytes(unsigned char count=1);

    
};



#endif