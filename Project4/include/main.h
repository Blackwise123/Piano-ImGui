#pragma once
#include "imgui.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

//piano layout
//num of oct
const int OCT = 2;
//white width key
const float W_W_K = 30;
//white height key
const float W_H_K = 120;
//black width key
const float B_W_K = 20;
//black height key
const float B_H_K = 80;

//state of piano key
struct PianoKey {
    //midi num
    int mNote;
    //note name(C4)
    string note;
    //is it black?
    bool isBlack;
    //is the key pressed
    bool ifpress;
    //is it highlighted
    bool highlighted;
    //color of highlight
    ImU32 highlightColor;
    //keyboard emun (similar to const int but alr declared) (http://www.fifi.org/doc/libsdl1.2-dev/docs/html/guideinputkeyboard.html)
    SDL_Keycode keyBinding;
};


//state of audio
struct PianoSound {
    //data type for mixer for short sound (https://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php)
    Mix_Chunk* sound;
    //track whether it is playing
    //true mean pressed, false is key is released
    bool Playing;
    //manage multiple sound playing simultanously
    //each playing sound need a channel
    //-1 is not playing
    int ch;
};
//keyboard map
const SDL_Keycode keyBindings[] = {
    SDLK_z,SDLK_s,SDLK_x,SDLK_d,SDLK_c,SDLK_v,
    SDLK_g,SDLK_b,SDLK_h,SDLK_n,SDLK_j,SDLK_m,
    //first oct
    SDLK_q,SDLK_2,SDLK_w,SDLK_3,SDLK_e,SDLK_r,
    SDLK_5,SDLK_t,SDLK_6,SDLK_y,SDLK_7,SDLK_u
    //second oct
};

//colors
const ImU32 wkeyC = IM_COL32(255, 255, 255, 255);
const ImU32 bkeyC = IM_COL32(0, 0, 0, 255);
const ImU32 pressWkeyC = IM_COL32(200, 200, 200, 255);
const ImU32 pressBkeyC = IM_COL32(100, 100, 100, 255);
const ImU32 scaleHC = IM_COL32(0, 255, 0, 100);
const ImU32 chordHC = IM_COL32(0, 0, 255, 100);



//global variables
//store states in array for multiple key pressed
extern vector<PianoKey> keys;
//toggle notenames
extern bool showname;
//check if audio system started
extern bool soundstarted;
//login screen or not
extern bool login;
//variable for sound management
//map midi to pianosound element
//prefer unorder over normal because it does not require offset calculation for index (https://www.geeksforgeeks.org/unordered_map-in-cpp-stl/)
//midinum,object
extern unordered_map<int, PianoSound> pianoSounds;
//list of pairs of username and password (https://www.geeksforgeeks.org/how-to-create-vector-of-pairs-in-cpp/)
extern vector<pair<string, string>> logins;
//declare functions
bool initSound();
bool loadSound();
//pass midi number to mp3 sound according to num
void playNote(int mNote);
void stopNote(int mNote);
void cleanSound();
//event is passed here for check if key a key is pressed
void handleKeyInput(const SDL_Event& event);
//setup pianokeys
void setupKeys();
//render piano
void render();
//getxpos
float getKeyX(int index);
//login screen
void renderlogin();