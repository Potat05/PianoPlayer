`
# [Auto Piano Player - Ver2.0](https://github.com/Potat05/PianoPlayer)  
  
[Garry's Mod](https://store.steampowered.com/app/4000/Garrys_Mod/)  
[Playable Piano](https://steamcommunity.com/sharedfiles/filedetails/?id=104548572)  
[Virtual Piano](https://virtualpiano.net/)  
  
  **THIS IS NOT TESTED ON VAC SECURE SERVERS**  
  **USE THIS ON VAC SECURE SERVERS AT YOUR OWN RISK!**  
  
  
# HELP
  
This was made for windows 10 x86  
If it doesn't work try installing Visual C++  
If it still doesn't work try searching the error online for any fixes  
If it still just wont work you can try to compile the code on your computer  
And if it still doesn't work you're out of luck i guess. . .  
  
  
# TODO  
  
- [x] Redo all the code 
- [x] Own song file format  
- [x] Custom delays for simple file format
- [ ] Transposition  
- [ ] Midi parser  
- [ ] Record your own song to play  
  
  
  
# Changelog  
  
  DD MM YYYY  
  
  * 19 6 2021
    * Remade the whole entire thing and the way songs are formatted
  * 16 8 2021
    * Support for more games (Like roblox)
  * 17 8 2021
    * Added AutoPianoPlayerOptions.txt options
      * key_delay (Used to increase the amount of time each key is pressed)
  * 18 8 2021
    * Fixed bug that made it impossible to exit the program when key_delay option is too high.
  * 19 8 2021
    * Song compressing for pianosave
    * Print action
  * 20 8 2021
    * New debug option, now you don't have to manually enable it
  * 21 8 2021 - 24 8 2021
    * Very basic MIDI song file playing
  * 25 8 2021
    * MIDI can now be played a bit better? (Delays somewhat work.)
  
  
  
# Piano File Simple Format  
  
This is the simple piano file format with less control than the binary format but easier to use  
This format is mostly just to port over the piano sheets from [Virtual Piano](https://virtualpiano.net/)  
They automatically get converted to song binary and to save it as a actual binary song file press NUMPAD8  
  
  
## FORMAT  
  
```
1 *Denote that this is a simple file format*
*Song name*
*Song description*
*Song delay 1* *Song delay 2* *Song delay 3* *Song delay 4*
*Song data*
```
  
* Song delays  
    1. Delay for anything that isn't ' ' or '|' if there isn't at ' ' or '|' ahead *Default: 200  
    2. Delay for ' ' *Default: 300  
    3. Delay for '|' *Default: 400  
    4. Delay for ' ' inside of '[]' *Default: 100  
  
* Brackets  
    1. Keys inside of brackets are played simultaneously  
    2. Spaces inside of brackets add delay #4  
  
* Curly Brackets  
    1. Delay is number inside of curly brackets  
  
  
  
Overworld theme (Super mario bros)
```
1
Overworld theme (Super mario bros)
Song Info.
200 300 400
[9u][9u] [9u] [9t][9u] [wo] |[5w] |[tw]|[9w]|[80]|[qe] [rw] [QE][qe] [8w][9u] [0o][qp] [9i][0o] [8u] tyr|[tw]|[9w]|[80]|[qe] [rw] [QE][qe] [8w][9u] [0o][qp] [9i][0o] [8u] tyr
```
  

# OTHER
  
You may modify this code in any way (Other than making it a virus) and re-upload it as long as you don't take all credit
  
  
[JS Midi Parser](https://github.com/colxi/midi-parser-js/blob/master/src/main.js) Massive help on learning how to parse the midi files  
  
[Virtual Piano Sheets](https://virtualpiano.net/music-sheets/) sheets 2  
[Virtual Piano Sheets](https://virtualpianosheets.com/) sheets 1  
[Virtual Piano Sheets](https://trello.com/b/n3cbomdF/virtual-piano-sheets) sheets 3
  
[Remove Line Breaks](https://removelinebreaks.net/) Remove line breaks in songs  



  
  