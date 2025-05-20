#include "MusicTheory.h"
using namespace std;

//global variables
int currentRoot=0;
int currentScaleIndex=0;
int currentChordIndex=0;
bool noteHighlighted=false;

//declare list of scales
const vector<Scale> scales = {
    {"Major", {0,2,4,5,7,9,11}},
    {"Natural Minor", {0,2,3,5,7,8,10}},
    {"Harmonic Minor", {0,2,3,5,7,8,11}},
    {"Melodic Minor", {0,2,3,5,7,9,11}},
    {"Pentatonic Major", {0,2,4,7,9}},
    {"Pentatonic Minor", {0,3,5,7,10}},
    {"Blues", {0,3,5,6,7,10}}
};

//declare list of chords
const vector<Chord> chords = {
    {"Major", {0,4,7}},
    {"Minor", {0,3,7}},
    {"Diminished", {0,3,6}},
    {"Augmented", {0,4,8}},
    {"Major 7th", {0,4,7,11}},
    {"Minor 7th", {0,3,7,10}},
    {"Dominant 7th", {0,4,7,10}},
    {"Suspended 4th", {0,5,7}},
    {"6th", {0,4,7,9}}
};

//highlight key according to choosen scale or chord
void highlightNote(int root, const vector<int>& intervals, ImU32 color) {
    //clear previous highlight
    //referencing to change the elements in class according to elements in the array
    for (PianoKey& key : keys)
    {
        key.highlighted = false;
        key.highlightColor = IM_COL32(0, 0, 0, 0);
    }

    //highlight according to chord or scale
    for (int i : intervals)
    {   //note to highlight
        int highlightnote = root + i;

        //highlight keys in interval for each octave
        for (PianoKey& key : keys)
        {
            if (key.mNote % 12 == highlightnote % 12)
            {
                key.highlighted = true;
                key.highlightColor = color;
            }
        }
    }
    //flag for clearing highlight
    noteHighlighted = true;
}

//clear highlight
void clearHighlight() {
    for (PianoKey& key : keys)
    {
        key.highlighted = false;
        key.highlightColor = IM_COL32(0, 0, 0, 0);
    }
    //cleared
    noteHighlighted = false;
}

//render chord and scale
void renderScaleChordUI() {
    //start new window called 
    ImGui::Begin("Scale and Chord");

    //root note slection
    //need ptr for compability
    const char* names[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    //create a dropdown selection with name "Root Name"
    //current item,list of items,size of items
    if (ImGui::Combo("Root Note", &currentRoot, names, IM_ARRAYSIZE(names)))
    {
        //check if there is highlight 
        if (noteHighlighted) {

            //highlight according to scale when selected
            highlightNote(currentRoot, scales[currentScaleIndex].intervals, scaleHC);

        }
    }

    //scale selection
    //converting to ptr for imgui api compability
    vector<const char*> sNames;
    for (const Scale& scale : scales)
    {   //convert str to ptr stg and store them
        sNames.push_back(scale.name.c_str());
    }
    //create dropdown
    //name,current item,first element of items,number of item
    if (ImGui::Combo("Scale", &currentScaleIndex, sNames.data(), sNames.size()))
    {
        //similar
        if (noteHighlighted)
        {

            highlightNote(currentRoot, scales[currentScaleIndex].intervals, scaleHC);

        }
    }

    //button to show highlight for scale
    if (ImGui::Button("Show Scale"))
    {
        highlightNote(currentRoot, scales[currentScaleIndex].intervals, scaleHC);
    }
    //line
    ImGui::Separator();

    //chord selection
    //similar
    vector<const char*> cName;
    for (const Chord& chord : chords)
    {
        cName.push_back(chord.name.c_str());
    }

    if (ImGui::Combo("Chord", &currentChordIndex, cName.data(), cName.size()))
    {

        if (noteHighlighted)
        {

            highlightNote(currentRoot, chords[currentChordIndex].intervals, chordHC);

        }
    }

    if (ImGui::Button("Show Chord"))
    {
        highlightNote(currentRoot, chords[currentChordIndex].intervals, chordHC);
    }

    ImGui::Separator();
    //clear highlight button
    if (ImGui::Button("Clear Highlighting"))
    {
        clearHighlight();
    }

    //finialize ui elements for the window for rendering
    ImGui::End();
}