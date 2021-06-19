
#include "keylist.h"

#include <windows.h>

using namespace std;


// TODO: Improve this jank function!
void Piano_KeyList::press_key(char key) {
    if(key >= 48 && key <= 57) { // 0-9
        keybd_event(key, 0, 0, 0);
        Sleep(key_time);
        keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
        return;
    }
    if(key >= 65 && key <= 90) { // A-Z
        keybd_event(VK_SHIFT, 0x10, 0, 0);
        keybd_event(key, 0, 0, 0);
        Sleep(key_time);
        keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        return;
    }
    if(key >= 97 && key <= 122) { // a-z
        keybd_event(key - 32, 0, 0, 0);
        Sleep(key_time);
        keybd_event(key - 32, 0, KEYEVENTF_KEYUP, 0);
        return;
    }

    // )!@#$%^&*(
    char uppk[10] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};

    for(int i=0; i < 10; i++) {
        if(key == uppk[i]) {
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(48 + i, 0, 0, 0);
            Sleep(key_time);
            keybd_event(48 + i, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
            return;
        }
    }
}

bool Piano_KeyList::press_note(char note, char octave) {
    for(int i=0; i < length; i++) {
        if(get_note(i) == note && get_octave(i) == octave) {
            press_key(get_key(i));
            return true;
        }
    }
    return false;
}

// http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html#BMA1_3
bool Piano_KeyList::press_midi(int value) {
    string notes = "cCdDefFgGaAb";
    char note = notes[value % 12];
    char octave = (value / 12) - 1;
    return press_note(note, octave);
}

int Piano_KeyList::get_key_index(char key) {
    for(int i=0; i < length; i++) {
        if(get_key(i) == key) return i;
    }
    return -1;
}