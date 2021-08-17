
#include <windows.h>
#include <iostream>
#include <fstream>

#include "player.h"
#include "parser.h"

using namespace std;


Piano_Player player;

string save_location = "_pianosave";
unsigned int save_max_size = 32 * 1024;


static void startAutoPiano() {

    while(true) {
        Sleep(1);

        if(GetAsyncKeyState(VK_NUMPAD1)) {
            cout << "Started auto piano." << endl;
            Sleep(100);
            player.start();

            while(player.running) {
                Sleep(1);

                player.step();

                if(GetAsyncKeyState(VK_NUMPAD2)) {
                    player.stop();
                    cout << "Stopped auto piano." << endl;
                }
                if(GetAsyncKeyState(VK_NUMPAD3)) exit(0);
                if(GetAsyncKeyState(VK_NUMPAD0)) player.debug = true;
                if(GetAsyncKeyState(VK_NUMPAD6)) player.time = 0;

            }

            if(player.finished) {
                cout << "Got to the end of the song. Stopped auto piano." << endl;
                cout << "Play time: " << player.total_time / 1000 << "sec" << endl;
            }
        }


        if(GetAsyncKeyState(VK_NUMPAD3)) exit(0);
        if(GetAsyncKeyState(VK_NUMPAD0)) player.debug = true;
        if(GetAsyncKeyState(VK_NUMPAD9)) {
            if(player.save(save_location, save_max_size)) cout << "Saved paino sheet to \"" << save_location << ".txt\"." << endl;
            else cout << "Cannot save piano sheet! File size exceeds " << (save_max_size / 1024) << "KB" << endl;
            Sleep(1000);
            exit(0);
        }



    }
    
}


Piano_Parser default_song() {
    Piano_Parser parser;
    // TODO: move this to a seperate file and make it binary format to hopefully save space!
    parser.parse_song("1\nSuper Mario Odyssey Main Theme (Super Mario Odyssey)\nSong Info.\n200 300 400\n[sl]|[ka][sl][oh]| [851]| ty[Y%]YO[8(%] oiY[i^]yiP[q^9]|P [ws8]|||[toh]| s| [tpd]| j [toh] s [tpd]gl[toh]|||[toh]| s| [tpd]| j [toh] s [tpj]kl[ypih]| [yoa]| [tqj]| s| [ywd]| fg[uha]| fh[zsY]|l|j [tqhe]|s| [q^9] s h|g[ywth]|||[ywr]|||[wo8]|[w8]t [w8] [ye8]|[e8]|[pe8] [wo8] [wt8] [ye8]i[e8]s[wo8]|[w8]|[w8] [wo8]|[w8]t [w8] [ye8]|[e8]|[pe8] [wo8] [wt8] [tp(]a[t(]s[qoe9]| [wr9]| [p84]|[84]t [84] [y95]|[95] u[95]i[wo8]|[w8] u[w8]o[td(]|[ts(]|[tp(] [wo8] [51]iu[te9] [62]t[sq^] [^4] [qi^][u0][t8][t51]|||t [qj]tip[sl]| [h0]wt[us][sl]| [g9]ey[if]g[pl] g [f0]wtus| [tqed]| j| [wts0]| h l [zyiPE]|lzlJ|jJjg|fgfs p [o5]9wryoadh|||[vuto]cxz l [okda] z v z [spmi]nbv c [xoda]| z| [splf]klz| [olda]klb m [vspi]|||[oda]|||[h8][wg][tf][ud] s [a5]9dw[rh] z [l4][k8][qj][he] g [f^]qE[id] fg[h3]70wjk[z@]$8[l(]kl[h2]69[sq]| [h84%]gD[g^95]dJ[sl]|[ka][sl][oh]| [851]| sd[D%]DH[8(%] hgD[g^]dgJ[q^9]|J [wl8]");
    return parser;
}


// There has to be a easier way of doing this.
void options(string file_location) {
    ifstream file(file_location);
    string file_string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Find each option & apply
    string option;
    string value;
    for(int i=0; i < (int)file_string.length(); i++) {
        // No option / comment
        if(file_string[i] == '#' || file_string[i] == ' ') {
            while(file_string[i] != '\n') i++;
        }

        if(file_string[i] == '\n') continue;

        // Get option
        while(file_string[i] != '=') option += file_string[i++];
        i++;
        while(file_string[i] != '\n' && i < (int)file_string.length()) value += file_string[i++];

        // Set option

        if(option == "key_time") {
            stringstream valss(value);
            int valnum = 0;
            valss >> valnum;
            player.keys.key_time = valnum;
        }



        // cout << option << "=" << value << endl;



    }

}



int main(int argc, char *argv[]) {

    SetConsoleTitle(TEXT("Auto Piano Player - https://github.com/Potat05/PianoPlayer"));
	

    options("AutoPianoPlayerOptions.txt");


    if(argc == 1) {
        player.data = default_song().data;
        cout << "No file given! Using default song." << endl;
    } else {
        int argp = 1;
        if(argc > 2) argp = rand() % (argc - 1) + 1;
        Piano_Parser parser;
        if(parser.parse_file(argv[argp])) {;
            player.data = parser.data;
            cout << "Loaded File " << argv[argp] << endl;
        } else {
            player.data = default_song().data;
            cout << "Error loading file! Using default song" << endl;
        }
    }



    // Init song
    player.load();




    cout << "========== PianoPlayer - V1.0 ==========" << endl;
    cout << "[ SONG ]" << endl;
    cout << player.name << endl;
    cout << player.description << endl;
    cout << endl;
    // cout << "Estimated time: " << (player.total_time() / 1000) << "sec" << endl;
    // cout << endl;
    cout << "[ HOTKEYS ]" << endl;
    cout << "NUMPAD1 = Start playing song" << endl;
    cout << "NUMPAD2 = Stop playing song" << endl;
    cout << "NUMPAD3 = Close program" << endl;
    cout << "NUMPAD6 = Skip through song" << endl;
    cout << endl;
    cout << "NUMPAD9 = Save song as binary \"" << save_location << ".txt\" & close program" << endl;
    cout << "NUMPAD0 = Enable debug mode" << endl;
    cout << "========================================" << endl;
    cout << "[ LOG ]" << endl;

    

    startAutoPiano();
}