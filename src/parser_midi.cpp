
#include "parser.h"

#include <sstream>
#include <iostream>

#include "globals.h"

using namespace std;



bool Piano_Parser::parse_midi(File_System midi) {
    midi.reset();

    // MIDI header validation
    if(midi.read_int() != 0x4D546864) {
        if(debug) cout << "MIDI header validation failed!" << endl;
        return false;
    }


    // Initial values
    data.clear();
    data.write_char('2');
    data.write_int(0x79BDAC3F);

    unsigned char song_data_type = 0;






    midi.read_int(); // Header size (unused)
    unsigned short format_type = midi.read_short();
    unsigned short track_count = midi.read_short();

    bool time_div_mode = false;
    unsigned short time_div;
    unsigned char time_div1 = midi.read_char();
    unsigned char time_div2 = midi.read_char();
    if(time_div1 > 128) {
        time_div_mode = true;
        time_div1 -= 128;
    } else {
        time_div = time_div1*256 + time_div2;
    }

    unsigned int tempo = 500000;
    unsigned int ticks_per_beat = 120;



    // Load first track


    // Track header validation
    unsigned int header = midi.read_int();
    if(header != 0x4D54726B) {
        if(debug) cout << "MIDI track header validation failed!" << endl;
        return false;
    }

    midi.read_int(); // Track size (unused)
    unsigned int event_counter = 0;
    bool end_of_track = false;
    unsigned char status_byte;
    unsigned char last_status_byte;

    while(!end_of_track) {
        event_counter++;

        if(midi.eof) break;

        unsigned long long int delta_time = midi.read_vlv();

        status_byte = midi.read_char();
        if(status_byte >= 128) {
            last_status_byte = status_byte;
        } else {
            status_byte = last_status_byte;
            midi.pointer--;
        }

        if(status_byte == 0xFF) { // Meta action
            unsigned char meta_type = midi.read_char();
            unsigned long long int meta_length = midi.read_vlv();

            switch(meta_type) {
                case 0x2f: // End of track
                    end_of_track = true;
                    break;
                case 0x08:
                case 0x0A:
                case 0x01: // Text event
                case 0x02: { // Copyright
                    // Set name & description
                    if(song_data_type == 0) {
                        string name = midi.read_string(meta_length);
                        data.write_char((unsigned char)name.length());
                        data.write_string(name);
                        song_data_type = 1;
                    } else if(song_data_type == 1) {
                        string description = midi.read_string(meta_length);
                        data.write_short((unsigned short)description.length());
                        data.write_string(description);
                        song_data_type = 2;
                    }
                    break; }
                case 0x03:
                case 0x04: // Instrument name
                case 0x05: // Lyrics
                case 0x07: // Cue point
                case 0x06: // Marker
                    midi.read_string(meta_length);
                    break;
                case 0x21: // MIDI port
                case 0x59: // Key signature
                    midi.read_bytes(meta_length);
                    break;
                case 0x51: // Set tempo
                    tempo = midi.read_bytes(meta_length);
                    break;
                case 0x54: // SMPTE offset
                    data.read_bytes(5);
                    break;
                case 0x58: // Time signature
                    midi.read_int();
                    break;
                default:
                    if(debug) cout << "Unknown MIDI meta event! 0x" << hex << (int)meta_type << " at " << dec << midi.pointer << endl;
                    midi.read_bytes(meta_length);
                    break;
            }
        } else { // Regular action

            // If no song name or description found before actions
            if(song_data_type == 0) {
                // Use file name as song name
                string name = "";
                for(int i=midi.name.rfind("\\")+1; i < (int)midi.name.length(); i++) {
                    if(midi.name[i] == '.') break;
                    name += (midi.name[i] == '_' ? ' ' : midi.name[i]);
                }
                data.write_char((unsigned char)name.length());
                data.write_string(name);
                song_data_type = 1;
            }
            if(song_data_type == 1) {
                string description = "Unknown.";
                data.write_short((unsigned short)description.length());
                data.write_string(description);
                song_data_type = 2;
            }

            unsigned char type = status_byte >> 4;
            unsigned char channel = status_byte & 0x0F;

            switch(type) {
                case 0xF: // System exlusive
                    if(debug) cout << "System exclusive event! 0xF at " << midi.pointer << ",skipping. . ." << endl;
                    midi.read_bytes(midi.read_vlv());
                    break;
                case 0xA: // Note aftertouch
                case 0xB: // Controller
                case 0xE: // Pitch blend
                case 0x8: // Note off
                    midi.read_short();
                    break;
                case 0x9: { // Note on
                    unsigned char piano_key = midi.read_char();
                    if(midi.read_char() != 0x50) break;

                    unsigned int time = (tempo / 1000) * delta_time / ticks_per_beat;
                    cout << time << endl;
                    addDelay(time);

                    data.write_char(0x41);
                    data.write_char(piano_key);
                    break; }
                case 0xC: // Program change
                case 0xD: // Channel aftertouch
                    midi.read_char();
                    break;
                default:
                    if(debug) cout << "Unknown event! 0x" << hex << (int)type << " at " << dec << midi.pointer << ". Stopped reading." << endl;
                    return false;
                    break;
            }
        }

    }





    return true;

}
