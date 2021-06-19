#ifndef KEYLIST_H
#define KEYLIST_H

#include <sstream>

using namespace std;

    
struct Piano_KeyList {
    // Key = "*button**note**octave*"
    int length = 62;
    string keys[62] = {
        "1c2", "!C2", "2d2", "@D2", "3e2", "4f2", "$F2", "5g2", "%G2", "6a2", "^A2", "7b2",
        "8c3", "*C3", "9d3", "(D3", "0e3", "qf3", "QF3", "wg3", "WG3", "ea3", "EA3", "rb3",
        "tc4", "TC4", "yd4", "YD4", "ue4", "if4", "IF4", "og4", "OG4", "pa4", "PA4", "ab4",
        "sc5", "SC5", "dd5", "DD5", "fe5", "gf5", "GF5", "hg5", "HG5", "ja5", "JA5", "kb5",
        "lc6", "LC6", "zd6", "ZD6", "xe6", "cf6", "CF6", "vg6", "VG6", "ba6", "BA6", "nb6",
        "mc7"
    };
    int key_time = 10; // Keep this at 10ms to make it work in gmod

    char get_key(int ind=0) { return keys[ind][0]; }
    char get_note(int ind=0) { return keys[ind][1]; }
    int get_octave(int ind=0) { return keys[ind][2] - '0'; }
    int get_key_index(char key);

    void press_key(char key);
    bool press_note(char note, char octave);
    bool press_midi(int value);
};


#endif