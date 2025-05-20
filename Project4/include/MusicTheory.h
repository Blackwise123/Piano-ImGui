#pragma once
#include "main.h"
using namespace std;

//state of scale
struct Scale {
    //display name
    string name;
    //semitone interval from root
    //eg. WWHWWWH ,half mean no skip ,whole mean a key skiped
    vector<int> intervals;
};

//state of chord
struct Chord {
    //display name
    string name;
    //similar to scale
    vector<int> intervals;
};



//global variables
// used by main and music theory
//track if key are highlighted
extern bool noteHighlighted;
//use by music theory
//C shown in the tab at start (check vector<Scale> scales)
extern int currentRoot;
//used by music theory only
//select which scale eg. major
extern int currentScaleIndex;
extern int currentChordIndex;
//list of predefined scales
extern const vector<Scale> scales;
//list of predefined chords
extern const vector<Chord> chords;



//functions
//pass the root note,prevent copying of intervals,colors
void highlightNote(int root, const vector<int>& intervals, ImU32 color);
void clearHighlight();
//render scale and chord ui
void renderScaleChordUI();