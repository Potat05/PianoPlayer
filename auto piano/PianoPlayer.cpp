
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// https://removelinebreaks.net/

bool running = false;

// DEFAULT SONG
string songName = "Super Mario Odyssey Main Theme";
string songDiscription = "Super Mario Odyssey Main Theme (Super Mario Odyssey)";
int pianoDelays[3] = {200, 300, 400};
int pianoInd = 0;
int keyTime = 10;
string pianoList = "[sl]|[ka][sl][oh]| [851]| ty[Y%]YO[8(%] oiY[i^]yiP[q^9]|P [ws8]|||[toh]| s| [tpd]| j [toh] s [tpd]gl[toh]|||[toh]| s| [tpd]| j [toh] s [tpj]kl[ypih]| [yoa]| [tqj]| s| [ywd]| fg[uha]| fh[zsY]|l|j [tqhe]|s| [q^9] s h|g[ywth]|||[ywr]|||[wo8]|[w8]t [w8] [ye8]|[e8]|[pe8] [wo8] [wt8] [ye8]i[e8]s[wo8]|[w8]|[w8] [wo8]|[w8]t [w8] [ye8]|[e8]|[pe8] [wo8] [wt8] [tp(]a[t(]s[qoe9]| [wr9]| [p84]|[84]t [84] [y95]|[95] u[95]i[wo8]|[w8] u[w8]o[td(]|[ts(]|[tp(] [wo8] [51]iu[te9] [62]t[sq^] [^4] [qi^][u0][t8][t51]|||t [qj]tip[sl]| [h0]wt[us][sl]| [g9]ey[if]g[pl] g [f0]wtus| [tqed]| j| [wts0]| h l [zyiPE]|lzlJ|jJjg|fgfs p [o5]9wryoadh|||[vuto]cxz l [okda] z v z [spmi]nbv c [xoda]| z| [splf]klz| [olda]klb m [vspi]|||[oda]|||[h8][wg][tf][ud] s [a5]9dw[rh] z [l4][k8][qj][he] g [f^]qE[id] fg[h3]70wjk[z@]$8[l(]kl[h2]69[sq]| [h84%]gD[g^95]dJ[sl]|[ka][sl][oh]| [851]| sd[D%]DH[8(%] hgD[g^]dgJ[q^9]|J [wl8]";




int StrToInt(string str) {
    stringstream ss(str);
    int x = 0;
    ss >> x;
    return x;
}




void PressKey(char key) {
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


int nextKey() {

    if(pianoInd >= pianoList.length() || pianoInd < 0) {
        running = false;
        cout << "ERROR: Piano index is outside of piano list range!" << endl;
        return 1000;
    }

    char pianoChar = pianoList[pianoInd];

    if(pianoChar == ' ') {
        pianoInd++;
        return pianoDelays[1];
    }
    if(pianoChar == '|') {
        pianoInd++;
        return pianoDelays[2];
    }

    if(pianoChar == '[') {
        int lInd = pianoInd;
        do {
            pianoInd++;
            pianoChar = pianoList[pianoInd];
            PressKey(pianoChar);
            if(pianoInd >= pianoList.length()) {
                running = false;
                cout << "ERROR: Open bracked loop detected!   startInd: " << lInd << endl;
                return 1000;
            }
        } while(pianoChar != ']');

        pianoInd++;
        char nChar = pianoList[pianoInd];
        if(nChar == ' ' || nChar == '|') {
            return 0;
        } else {
            return pianoDelays[0] - (pianoInd - lInd)*keyTime;
        }
    }

    PressKey(pianoChar);

    pianoInd++;
    char nChar = pianoList[pianoInd];
    if(nChar == ' ' || nChar == '|') {
        return 0;
    } else {
        return pianoDelays[0] - keyTime;
    }

}




static void startAutoPiano() {

	while (true) {
        Sleep(1);

		if (GetAsyncKeyState(VK_NUMPAD3)) {
			cout << "Exited Program." << endl;
			Sleep(500);
			exit(0);
		}

		if (GetAsyncKeyState(VK_NUMPAD1)) {
			cout << "Started auto piano." << endl;
			Sleep(100);
			running = true;
            pianoInd = 0;

			while (true) {
                if(pianoInd >= pianoList.length()) {
                    running = false;
                    cout << "Got to end of piano sheet. Stopped auto piano." << endl;
                }

                if (!running) {
					break;
				}


                Sleep(nextKey());




				if (GetAsyncKeyState(VK_NUMPAD2)) {
					running = false;
					cout << "Stopped auto piano." << endl;
				}
				if (GetAsyncKeyState(VK_NUMPAD3)) {
                    cout << "Exited Program." << endl;
                    Sleep(500);
                    exit(0);
				}
			}
		}


	}
    
}




int main(int argc, char* argv[]) {
    SetConsoleTitle("Auto Piano Player - Ver0.1");


    // Load from dragged file
    if(argc == 2) {
        cout << "Loaded File. " << argv[1] << endl;

        ifstream pianoFile(argv[1]);
        if(pianoFile) {
            ostringstream oss;
            oss << pianoFile.rdbuf();
            string str = oss.str();
            
            // Split into array on each new line
            string arr[7];
            stringstream ss(str);
            int i = 0;
            while(ss.good()) {
                string substr;
                getline(ss, substr, '\n');
                arr[i] = substr;
                i++;
            }

            songName = arr[0];
            songDiscription = arr[1];
            pianoDelays[0] = StrToInt(arr[2]);
            pianoDelays[1] = StrToInt(arr[3]);
            pianoDelays[2] = StrToInt(arr[4]);
            keyTime = StrToInt(arr[5]);
            pianoList = arr[6];


        } else {
            cout << "Error loading file! Using default song." << endl;
        }
    } else {
        cout << "No file given! Using default song." << endl;
    }


	cout << "============  Auto Piano Player - Ver0.1  ============"<< endl;
    cout << "[ SONG ]" << endl;
    cout << songName << endl;
    cout << songDiscription << endl;
    cout << "Length: " << pianoList.length() << endl;
    cout << endl;
	cout << "[ HOTKEYS ]" << endl;
	cout << "NumPad1 = START" << endl;
	cout << "NumPad2 = STOP" << endl;
	cout << "NumPad3 = CLOSE" << endl;
	cout << "======================================================" << endl;
	cout << "[ LOG ] " << endl;



	startAutoPiano();
}
