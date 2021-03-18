# PianoPlayer
Garry's Mod Auto Piano Player  
  
My first C++ project!  
  
  
  
# HOW TO USE
Drag a song from the songs folder onto PianoPlayer.exe  
  
NUMPAD1 to start the song  
NUMPAD2 to stop the song  
NUMPAD3 to close the program  
  
(If keybinds don't work make sure you have num lock on!)  
  
  
  
# TODO
- [ ] Allow multiple keys to be played at once (Right now it just plays every one in a group really fast)  
- [ ] GUI  
- [ ] Transposition (Move keys up or down in pitch)  
  
  
  
# OTHER
[VirtualPiano](https://virtualpiano.net/) to find songs.  
[RemoveLineBreaks](https://removelinebreaks.net/) to put songs onto one line so the code can read it.  
  
  
  
# SONG FILE
  
The song file format.  
  
=============== START OF FILE ===============  
Song Name  
Song Description  
Delay 1 (Delay for anything that isnt ' ' or '|' but no delay if ' ' or '|' is ahead)  
Delay 2 (Delay for ' ')  
Delay 3 (Delay for '|')  
Key Time (The amount of time the key is pressed *Keep this above 0 if you want it to work for Garry's Mod*)  
Song Data (The notes in the song)  
================ END OF FILE ================  
  
  
