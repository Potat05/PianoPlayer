
#include "presskeys.h"


void PressKey(char key, int keyTime) {
    
    int keyInt = (int)key;
    // cout << key << "   " << keyInt << endl;
    
    if(keyInt >= 48 && keyInt <= 57) { // 0-9
        keybd_event(keyInt, 0, 0, 0);
        Sleep(keyTime);
        keybd_event(keyInt, 0, KEYEVENTF_KEYUP, 0);
        return;
    }
    if(keyInt >= 65 && keyInt <= 90) { // A-Z
        keybd_event(VK_SHIFT, 0x10, 0, 0);
        keybd_event(keyInt, 0, 0, 0);
        Sleep(keyTime);
        keybd_event(keyInt, 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        return;
    }
    if(keyInt >= 97 && keyInt <= 122) { // a-z
        keybd_event(keyInt-32, 0, 0, 0);
        Sleep(keyTime);
        keybd_event(keyInt-32, 0, KEYEVENTF_KEYUP, 0);
        return;
    }
    switch(keyInt) {
        case 41: // )
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(48, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(48, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 33: // !
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(49, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(49, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 64: // @
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(50, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(50, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 35: // #
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(51, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(51, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 36: // %
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(52, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(52, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 37: // ^
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(53, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(53, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 94: // &
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(54, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(54, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 38: // *
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(55, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(55, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 42: // (
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(56, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(56, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
        case 40: // )
            keybd_event(VK_SHIFT, 0x10, 0, 0);
            keybd_event(57, 0, 0, 0);
            Sleep(keyTime);
            keybd_event(57, 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0x10, KEYEVENTF_KEYUP, 0);
        break;
    }

}