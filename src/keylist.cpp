
#include "keylist.h"

#include <windows.h>

#include <iostream>

using namespace std;



// I'm hopeless, what did I create?
void Piano_KeyList::input_key(char keyParam, bool upper) {
    // Hold shift
    if(upper) {
        unsigned int mappedkey2 = MapVirtualKey(VK_SHIFT, 0);
        INPUT input2;
        input2.type = INPUT_KEYBOARD;
        input2.ki.dwFlags = KEYEVENTF_SCANCODE;
        input2.ki.wScan = mappedkey2;
        SendInput(1, &input2, sizeof(input2));
    }
    // Press key
    short key1 = VkKeyScan(keyParam);
    unsigned int mappedkey1 = MapVirtualKey(LOBYTE(key1), 0);
    INPUT input1;
    input1.type = INPUT_KEYBOARD;
    input1.ki.dwFlags = KEYEVENTF_SCANCODE;
    input1.ki.wScan = mappedkey1;
    SendInput(1, &input1, sizeof(input1));
    Sleep(key_time);
    input1.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    SendInput(1, &input1, sizeof(input1));

    // Release shift
    if(upper) {
        unsigned int mappedkey2 = MapVirtualKey(VK_SHIFT, 0);
        INPUT input2;
        input2.type = INPUT_KEYBOARD;
        input2.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        input2.ki.wScan = mappedkey2;
        SendInput(1, &input2, sizeof(input2));
    }
}

// Good enough, I guess.
void Piano_KeyList::press_key(char key) {
    if(key >= 48 && key <= 57) { // 0-9
        input_key(key, false);
        return;
    }
    if(key >= 65 && key <= 90) { // A-Z
        input_key(key, true);
        return;
    }
    if(key >= 97 && key <= 122) { // a-z
        input_key(key - 32, false);
        return;
    }

    // )!@#$%^&*(
    char uppk[10] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '('};

    for(int i=0; i < 10; i++) {
        if(key == uppk[i]) {
            input_key(48 + i, true);
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