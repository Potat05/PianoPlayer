
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include "./presskeys.h"
using namespace std;

string version = "0.3";

bool running = false;

// DEFAULT SONG
string song_name = "Super Mario Odyssey Main Theme (Super Mario Odyssey)";
string song_description = "Song Info.";
int song_delays[3] = {200, 300, 400};
int song_index = 0;
int song_keytime = 10;
string song_notes = "[sl]|[ka][sl][oh]| [851]| ty[Y%]YO[8(%] oiY[i^]yiP[q^9]|P [ws8]|||[toh]| s| [tpd]| j [toh] s [tpd]gl[toh]|||[toh]| s| [tpd]| j [toh] s [tpj]kl[ypih]| [yoa]| [tqj]| s| [ywd]| fg[uha]| fh[zsY]|l|j [tqhe]|s| [q^9] s h|g[ywth]|||[ywr]|||[wo8]|[w8]t [w8] [ye8]|[e8]|[pe8] [wo8] [wt8] [ye8]i[e8]s[wo8]|[w8]|[w8] [wo8]|[w8]t [w8] [ye8]|[e8]|[pe8] [wo8] [wt8] [tp(]a[t(]s[qoe9]| [wr9]| [p84]|[84]t [84] [y95]|[95] u[95]i[wo8]|[w8] u[w8]o[td(]|[ts(]|[tp(] [wo8] [51]iu[te9] [62]t[sq^] [^4] [qi^][u0][t8][t51]|||t [qj]tip[sl]| [h0]wt[us][sl]| [g9]ey[if]g[pl] g [f0]wtus| [tqed]| j| [wts0]| h l [zyiPE]|lzlJ|jJjg|fgfs p [o5]9wryoadh|||[vuto]cxz l [okda] z v z [spmi]nbv c [xoda]| z| [splf]klz| [olda]klb m [vspi]|||[oda]|||[h8][wg][tf][ud] s [a5]9dw[rh] z [l4][k8][qj][he] g [f^]qE[id] fg[h3]70wjk[z@]$8[l(]kl[h2]69[sq]| [h84%]gD[g^95]dJ[sl]|[ka][sl][oh]| [851]| sd[D%]DH[8(%] hgD[g^]dgJ[q^9]|J [wl8]";

int song_length = song_notes.length();
int song_nextNoteTime = 0;




static int StrToInt(string str) {
    stringstream ss(str);
    int x = 0;
    ss >> x;
    return x;
}

static void killProgram() {
    cout << "Exited Program." << endl;
	Sleep(500);
	exit(0);
}





static int nextKey() {

    if(song_index >= song_length || song_index < 0) {
        running = false;
        cout << "ERROR: Piano index is outside of piano list range!" << endl;
        return 1000;
    }

    char note = song_notes[song_index];

    if(note == ' ') {
        song_index++;
        return song_delays[1];
    }
    if(note == '|') {
        song_index++;
        return song_delays[2];
    }
    if(note == '{') {
        song_index++;
        string numstr = "";
        while(song_notes[song_index] != '}') {
            numstr += song_notes[song_index];
            song_index++;
        }
        song_index++;
        return StrToInt(numstr);
    }

    if(note == '[') {
        int lInd = song_index;
        do {
            song_index++;
            note = song_notes[song_index];
            PressKey(note, song_keytime);
            if(song_index >= song_length) {
                running = false;
                cout << "ERROR: Open bracked loop detected!   startInd: " << lInd << endl;
                return 1000;
            }
        } while(note != ']');

        song_index++;
        note = song_notes[song_index];
        if(note == ' ' || note == '|') {
            return 0;
        } else {
            return song_delays[0] - (song_index - lInd)*song_keytime;
        }
    }

    PressKey(note, song_keytime);

    song_index++;
    note = song_notes[song_index];
    if(note == ' ' || note == '|') {
        return 0;
    } else {
        return song_delays[0] - song_keytime;
    }

}

static void pianoTick() {
    int t = clock();
    if(t >= song_nextNoteTime) {
        song_nextNoteTime = t + nextKey();
    }
}


static void startAutoPiano() {

	while (true) {
        Sleep(1);

		if (GetAsyncKeyState(VK_NUMPAD3)) {
			killProgram();
		}

		if (GetAsyncKeyState(VK_NUMPAD1)) {
			cout << "Started auto piano." << endl;
			Sleep(100);
			running = true;
            song_index = 0;

			while (true) {
                Sleep(1);

                if(song_index >= song_length) {
                    running = false;
                    cout << "Got to end of piano sheet. Stopped auto piano." << endl;
                }

                if (!running) {
					break;
				}

                

                pianoTick();



				if (GetAsyncKeyState(VK_NUMPAD2)) {
					running = false;
					cout << "Stopped auto piano." << endl;
				}
				if (GetAsyncKeyState(VK_NUMPAD3)) {
                    killProgram();
				}
			}
		}


	}
    
}



static void loadSongFromString(string str) {
    string arr[7];
    stringstream ss(str);
    int i = 0;
    while(ss.good()) {
        string substr;
        getline(ss, substr, '\n');
        arr[i] = substr;
        i++;
    }

    song_name = arr[0];
    song_description = arr[1];
    song_delays[0] = StrToInt(arr[2]);
    song_delays[1] = StrToInt(arr[3]);
    song_delays[2] = StrToInt(arr[4]);
    song_keytime = StrToInt(arr[5]);
    song_notes = arr[6];
    song_length = song_notes.length();
}


int main(int argc, char* argv[]) {

    SetConsoleTitle(TEXT("Auto Piano Player - https://github.com/Potat05/PianoPlayer"));


    // Load from dragged file
    if(argc == 2) {
        cout << "Loaded File. " << argv[1] << endl;

        ifstream song_file(argv[1]);
        if(song_file) {
            ostringstream oss;
            oss << song_file.rdbuf();
            string str = oss.str();
            loadSongFromString(str);
        } else {
            cout << "Error loading file! Using default song." << endl;
        }
    } else {
        cout << "No file given! Using default song." << endl;
    }



	cout << "============  Auto Piano Player - Ver"<< version <<"  ============"<< endl;
    cout << "[ SONG ]" << endl;
    cout << song_name << endl;
    cout << song_description << endl;
    cout << "Length: " << song_length << endl;
    cout << endl;
	cout << "[ HOTKEYS ]" << endl;
	cout << "NumPad1 = START" << endl;
	cout << "NumPad2 = STOP" << endl;
	cout << "NumPad3 = CLOSE" << endl;
	cout << "======================================================" << endl;
	cout << "[ LOG ] " << endl;





	startAutoPiano();
}
