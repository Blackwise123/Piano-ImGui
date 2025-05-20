#include "main.h"
#include "MusicTheory.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
using namespace std;


//global variable declaration
vector<PianoKey> keys;
bool showname=true;
unordered_map<int,PianoSound> pianoSounds;
vector<pair<string, string>> logins = { {"s", "s"}, {"a", "a"} };
bool soundstarted=false;
//check whether login
bool login=false;
//buffer for user and pass input
char userinput[64] = ""; 
char passinput[64] = ""; 




//helper function get lenght from orgin to top left (wwk*index)
//eg. 0*30=0,1*30=30,2*30=60
float getKeyX(int index) {
    //first key*widith of white key
    return index*W_W_K;
}

//setup pianokey with keyboard binding
void setupKeys() {
    //declare note names
    const char* names[] = { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

    //create for key for first oct then second oct
    for (int oct=0; oct<OCT ; oct++) 
    {
        //C(60)+(num of oct * keys in octave)
        int initNote = 60 + (oct*12); 

        //numbers of key in octave
        for (int j=0 ; j<12 ; j++) 
        {
            //increase midinum
            int mNote = initNote+j;
            //start at oct 4 (C4)
            string name = names[j]+to_string(4+oct);
            //Making key black in this order in a octave (C#, D#, F#, G#, A#)
            bool isBlack = (j==1 || j==3 || j==6 || j==8 || j==10);

            //prevent unpredictable behavior when no key is assign but can be remove and just declare
            SDL_Keycode keyBinding = SDLK_UNKNOWN;
            //counter according to num in octave
            int index = (oct*12)+j;
            if (index < sizeof(keyBindings)) 
            {
                //assign piano key a keyboard key
                keyBinding = keyBindings[index];
            }
            //seting up the key
            keys.push_back({mNote,name,isBlack,false,false,IM_COL32(0,0,0,0),keyBinding});
        }
    }
}
void renderlogin() {
    if (login) {
        return;
    }
    //window tab is fixed
    //name of tab,cannot be closed,cannot be resized and minizied
    ImGui::Begin("Login", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::Text("Input Username:");
    //label,variable,buffersize of input can also use sizeof()
    ImGui::InputText("##usernameInput", userinput, IM_ARRAYSIZE(userinput));

    ImGui::Text("Input Password:");
    //hide password
    ImGui::InputText("##passwordInput", passinput, IM_ARRAYSIZE(passinput), ImGuiInputTextFlags_Password);
    //need static because matchfound is true only when button is pressed
    static string i="";
    if (ImGui::Button("Login")) {
       bool matchfound=false;
       
        //check input against vector pairs of usernames and passwords
        for (const auto& cred : logins) 
        {
            
            //compare two string,string need to be change to ptr because it cannot convert by itself
            //(https://www.geeksforgeeks.org/strcmp-in-c/)
            if (strcmp(userinput, cred.first.c_str()) == 0 && strcmp(passinput, cred.second.c_str()) == 0) 
            {
                matchfound = true;
                login = true;
                //clear password when you relogin
                //variable,change byte of data to zero,caluculate the size of data
                //(https://www.geeksforgeeks.org/strcmp-in-c/)
                memset(passinput, 0, sizeof(passinput));
                break;
            }
        }
        if (!matchfound) {
            i = "Wrong password or username";
        }

    }
    //wrong password or username

        ImGui::Text(i.c_str());

 


    ImGui::End();
}



//render piano
void render() {
    //start new window
    ImGui::Begin("Piano");

    //if showname is true, hide notes
    if (ImGui::Button(showname ? "Hide notes":"Shown notes")) {
        //invert the flag
        showname=!showname;
    }

    //display keybinding
    ImGui::Separator();
    ImGui::Text("Use keyboard");
    ImGui::Text("1st oct: Z,S,X,D,C,V,G,B,H,N,J,M");
    ImGui::Text("2nd oct: Q,2,W,3,E,R,5,T,6,Y,7,U");
    
    //add line
    ImGui::Separator();

    //declaring an object with class ptr for drawing
    ImDrawList* drawList=ImGui::GetWindowDrawList();
    //declaring an object with struct(x,y)
    ImVec2 pos=ImGui::GetCursorScreenPos();

    //use ptr array for mortifying multiple state of keys 
    //data is revalent only when key is pressed or highlight
    vector<PianoKey*> wKeys;
    vector<PianoKey*> bKeys;
    
    //storing whitekey elements in form of PianoKey ptr
    for (int oct=0; oct<OCT; oct++) 
    {   //all wkeys in oct
        for (int i=0; i<7; i++) 
        {
            //Midi num according oct
            int initNote=60+(oct*12); //C4(60)
            //current midi num
            int mNote; 

            //Pattern is same for each oct
            //C,D,E,F,G,A,B
            switch (i) 
            {
            //+2,+2,+1,+2,+2,+2,+1
            case 0: mNote=initNote+0; break; 
            case 1: mNote=initNote+2; break; 
            case 2: mNote=initNote+4; break; 
            case 3: mNote=initNote+5; break; 
            case 4: mNote=initNote+7; break; 
            case 5: mNote=initNote+9; break; 
            case 6: mNote=initNote+11; break; 
            default: mNote=0; break;
            }

            
            for (PianoKey& key : keys) 
            {   //check all keys in pianokey if it is white and midi number is same
                if (!key.isBlack && key.mNote==mNote) 
                {
                    wKeys.push_back(&key);
                    break;
                }
            }
        }
    }

    //draw key for white
    for (int i=0; i<wKeys.size(); i++) 
    {   //accessing key form wKeys
        PianoKey* key=wKeys[i];
        //get lenght from orgin to top left (wwk*index)
        float keyX=getKeyX(i);

        // Draw white key
        //top left of white key from keyX (pos.x+keyX, pos.y)
        ImVec2 keyMin=ImVec2(pos.x+keyX, pos.y);
        //bottom right of white key from keyX (pos.x+keyX+W_W_K, pos.y+W_H_K)
        ImVec2 keyMax=ImVec2(pos.x+keyX+W_W_K, pos.y+W_H_K);

        //color key
        ImU32 keyColor;
        //dereferencing the pointer to check state of key and set color
        //pianokey& i=*key; (dereferancing using referance)
        //if (i.ifpress)
        if (key->ifpress) 
        {
            keyColor=pressWkeyC;
        }
        else if (key->highlighted) 
        {
            keyColor=key->highlightColor;
        }
        else 
        {
            keyColor=wkeyC;
        }

        //dereferencing to call function
        //fill rect
        drawList->AddRectFilled(keyMin, keyMax, keyColor);
        //outline rect
        drawList->AddRect(keyMin, keyMax, IM_COL32(0,0,0,255));

        //add note name
        if (showname) {
            //pos bottom left of text box,color,notename ptr
            drawList->AddText(ImVec2(keyMin.x+5, keyMin.y+W_H_K-20),IM_COL32(0,0,0,255),key->note.c_str());
        }
    }

    //draw blackkeys for each oct
    for (int oct=0; oct<OCT; oct++) 
    {
        //placed after white keys C,D,F,G,A
        //no key after E and B
        //7 wkeys in oct
        int octOffset=oct*7; 

        //pos of black keys
        //after C,D,F,G,A
        const int bkPos[]={0,1,3,4,5}; 

        //5 black key per octave
        for (int i=0; i<5; i++) 
        {   
            int wkIndex=octOffset+bkPos[i]; //white key index that have black key according oct
            //lenght from orgin to bottom left (wwk*index)
            float whiteKeyX=getKeyX(wkIndex);

            //lenght from orgin to bottom left of bk
            float blackKeyX=whiteKeyX+W_W_K-B_W_K/2;

            //midi according oct (60=c4)
            int initNote=60+(oct*12);
            //bkmidi
            int bkMidi;

            //map current midi to note
            // C#,D#,F#,G#,A#
            switch (i) 
            {
            case 0: bkMidi=initNote+1; break; 
            case 1: bkMidi=initNote+3; break; 
            case 2: bkMidi=initNote+6; break; 
            case 3: bkMidi=initNote+8; break; 
            case 4: bkMidi=initNote+10; break; 
            default: bkMidi=0; break;
            }

            //draw current black key
            for (PianoKey& key : keys) {
                if (key.isBlack && key.mNote==bkMidi) 
                {
                    //top left of bk (pos.x+blackKeyX, pos.y)
                    ImVec2 keyMin = ImVec2(pos.x+blackKeyX, pos.y);
                    //bottom right of bk (pos.x+blackKeyX+B_W_K, pos.y+B_H_K)
                    ImVec2 keyMax = ImVec2(pos.x+blackKeyX+B_W_K, pos.y+B_H_K);

                    //add black key to render
                    bKeys.push_back(&key);

                    //color when pressed
                    //similar
                    ImU32 keyColor;
                    if (key.ifpress) 
                    {
                        keyColor=pressBkeyC;
                    }
                    else if (key.highlighted) 
                    {
                        keyColor=key.highlightColor;
                    }
                    else 
                    {
                        keyColor=bkeyC;
                    }
                    //fill the rect
                    drawList->AddRectFilled(keyMin, keyMax, keyColor);
                    //outline the rect
                    drawList->AddRect(keyMin, keyMax, IM_COL32(0,0,0,255));

                    //note name
                    if (showname) 
                    {
                        drawList->AddText(ImVec2(keyMin.x+3, keyMin.y+B_H_K-20),IM_COL32(255,255,255,255), key.note.c_str());
                    }
                    break;
                }
            }
        }
    }

    //default space of piano tab
    ImGui::Dummy(ImVec2(OCT*7*W_W_K,W_H_K+10));
    //finialize rendering element to prepare for next rendering
    ImGui::End();
}

//play sound when key is pressed and stop sound when key is release
//keyboard input
void handleKeyInput(const SDL_Event& event) {
    //specific key pressed from sdl event
    SDL_Keycode keycode=event.key.keysym.sym;
    //flag to see if keyboardkey is pressed
    bool keyDown=(event.type == SDL_KEYDOWN);

    // Process the key for all piano keys with this binding
    for (PianoKey& key : keys) 
    {
        //check if any key from keybinding is pressed
        if (key.keyBinding == keycode) 
        {
            //if it is pressed
            if (keyDown && !key.ifpress) 
            {
                //change key state 
                key.ifpress=true;
                //make sound
                playNote(key.mNote);
            }
            //if key is not pressed
            else if (!keyDown && key.ifpress) 
            {
                //change key state
                key.ifpress = false;
                //stop sound
                stopNote(key.mNote);
            }
        }
    }
}

//setup sound 
bool initSound() {
    
    //initalize sdl
    if (SDL_Init(SDL_INIT_AUDIO) < 0) 
    {   
        //string is formated using %d and %s (https://www.gnu.org/software/libc/manual/html_node/Formatted-Output-Basics.html)
        printf("SDL audio could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    //initalize sdl_mixer
    //cd quality(sample rate in hz),default audio format,stero,chunk size(minimize lag in sound)
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    //allocate sound for each key
    Mix_AllocateChannels(24);
    //flag show sound setup
    soundstarted = true;
    return true;
}

//load mp3 file
bool loadSound() {
    //check if sound have alr setup
    if (!soundstarted && !initSound()) 
    {
        return false;
    }

    //first letters of mp3 filename
    const char* names[]={"A","Bb","B","C","Db","D","Eb","E","F","Gb","G","Ab"};

    //load sound from 60 to 83(C4 to B5) 2 oct
    for (int mNote=60; mNote<=83; mNote++) {
        //num for noteName (oct, A4)
        int oct=(mNote-12)/12; 
        //note in the oct
        int noteIndex=(mNote-12)%12;
        //two oct so need shift back by 1 oct
        //eg. 0=A,1=Bb.. before it hit 12 (note: Bb is A#)

        //file location including name
        string filename=string("./piano-mp3/")+names[noteIndex]+to_string(oct)+".mp3";

        //load audio file
        //filename need to be ptr because it is not string literal which it is encapsulated preventing conversion to ptr
        Mix_Chunk* sound=Mix_LoadWAV(filename.c_str());

        //debug
        if (sound == NULL) {
            printf("Failed to load sound for MIDI note %d! SDL_mixer Error: %s\n",mNote, Mix_GetError());
            return false;
            
        }

        //load sound
        //sound data,is playing,no channel
        pianoSounds[mNote] = { sound, false, -1 };
    }

    return true;
}

//play note 
void playNote(int mNote) {
    //check if the sound is initialized
    if (!soundstarted && !initSound()) {
        return;
    }
    //not login return
    if (login == false) {
        return;
    }
    //check if sound is loaded
    if (pianoSounds.empty() && !loadSound()) {
        return;
    }

    //check if it have the note
    //used auto or else
    //uordered_map<int,PianoSound>::iterator
    auto i=pianoSounds.find(mNote);
    //if no sound exit out of function
    if (i == pianoSounds.end()) {
        printf("No sound found for MIDI note %d\n", mNote);
        return;
    }

    //check if it is not same note is playing (in the second element)
    if (!i->second.Playing) 
    {   //set the channel in pianosounds to control the channel
        //any channel,sound,play one time
        i->second.ch=Mix_PlayChannel(-1, i->second.sound, 0);
        if (i->second.ch==-1) 
        {
            printf("Failed to play sound for MIDI note %d! SDL_mixer Error: %s\n",mNote, Mix_GetError());
            return;
        }
        //change the key state
        i->second.Playing=true;
    }
}

//stop note 
void stopNote(int mNote) {
    //check if sound started
    if (!soundstarted) 
    {
        return;
    }
    //check if any audio
    auto i = pianoSounds.find(mNote);
    //check if key is released or no channel is open or no sound
    if (i!=pianoSounds.end() && i->second.Playing && i->second.ch!=-1) 
    {   //stop sound at the channel
        Mix_HaltChannel(i->second.ch);
        //change key state
        i->second.Playing=false;
        i->second.ch=-1;
    }
}

//clean the sound channel
void cleanSound() {
    //check all pairs in pianoSounds
    for (auto& i : pianoSounds) {
        //make it readable
        PianoSound& sounds = i.second;
        //i.second.sound is whiether empty
        if (sounds.sound != NULL) {
            //empty it
            Mix_FreeChunk(sounds.sound);
            sounds.sound = NULL;
        }
    }
    //clear the vector
    pianoSounds.clear();
    //handle library that manages sound
    Mix_CloseAudio();
    //handle the direct hardware interface
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    //sound not started
    soundstarted = false;
}


int main(int argc, char* argv[]) {
    //initalize the sdl
    //video flage is for for rendering
    //SDL_INIT_TIMER could be add for timing the note if needed
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("error: %s\n", SDL_GetError());
        return -1;
    }

    //initalize sound
    if (!initSound())
    {
        printf("failed to start sound\n");

    }
    else {
        //load sound
        if (!loadSound())
        {
            printf("cannot load sound\n");

        }
    }

    //setup window
    //SDL_WINDOW_RESIZABLE allow user to resize
    //SDL_WINDOW_ALLOW_HIGHDPI enable high dpi support retina displays if needed
    //combine multiple flag with bitwise operator |
    SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    //title of window
    //SDL_WINDOWPOS_CENTERED center the windo
    //size of window
    //use the flags above
    SDL_Window* window=SDL_CreateWindow("Piano Learning Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768,flags);

    //failed exit
    if (window==NULL) 
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    //setup render
    //window where render take place
    //use first supported driver
    //sync rendering with monitor hz
    //SDL_RENDERER_ACCELERATED for consistent behavior to use gpu if needed
    SDL_Renderer* renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC| SDL_RENDERER_ACCELERATED);

    //failed exit
    if (renderer==NULL) 
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    //setup ImgGui context
    //verify imgui version compatibility
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //get input state such as mouse
    //display properties
    ImGuiIO& io=ImGui::GetIO();
    
    //arrow keys for ui navigation using mouse or keyboard
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //setup Imgui style
    ImGui::StyleColorsDark();

    //setup renderbackend
    //conect imgui with sdl window and event
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    //connect imgui with sdl render
    ImGui_ImplSDLRenderer2_Init(renderer);

    //setup pianokeys
    setupKeys();

    //main loop
    bool done = false;
    while (!done) 
    {   //hold current event
        SDL_Event event;

        //process all pending sdl events
        //return 0 if there is no event
        while (SDL_PollEvent(&event)) 
        {   
            //called before event for key board
            //handle input for ui element
            ImGui_ImplSDL2_ProcessEvent(&event);

            //exit loop when program is close
            if (event.type==SDL_QUIT)
                done = true;
            //event.window.event==SDL_WINDOWEVENT_CLOSE check if id of window being close match id of the windo
            //event.type==SDL_WINDOWEVENT check if event is window related
            if (event.type==SDL_WINDOWEVENT && event.window.event==SDL_WINDOWEVENT_CLOSE)
                done = true;

            //check key is pressed or released
            if (event.type==SDL_KEYDOWN || event.type==SDL_KEYUP) 
            {
                handleKeyInput(event);
            }
        }

        //start imgui frame
        //new imgui rendering frame for sdl render
        ImGui_ImplSDLRenderer2_NewFrame();
        //new imgui frame for sdl backend
        //update mouse position and button state so on
        ImGui_ImplSDL2_NewFrame();
        //new imgui frame for ui element
        ImGui::NewFrame();
        renderlogin();
        if (login) {
            //render piano ui
            render();

            //render scalechord ui
            renderScaleChordUI();
            //logout button
            if (ImGui::Button("logout")) {
                login = false;
            }

        }

        // finalize imgui rendering and prepare new data
        ImGui::Render();
        //set renderer scale 
        //horizontal scaling factor between phyiscal and logical pixels
        //vertical scalling factor 
        //image will not look distorted in high res
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        //set background to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        //clear to this color
        SDL_RenderClear(renderer);
        //draw ui from getdrawdata
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        //display the content from renderer's interal buffer to screen
        SDL_RenderPresent(renderer);
    }

    //cleanup
    cleanSound();
    //shutdown renderer back first
    //handle textures,rendering resource (imgui specific)
    ImGui_ImplSDLRenderer2_Shutdown();
    //shutdown input backend
    //handles input-related resources eg.mouse cursors(imgui specific)
    ImGui_ImplSDL2_Shutdown();
    //destroy main imgui context
    //handle imgui memory, widget ,data structure
    ImGui::DestroyContext();
    //destory textures and rendering resources
    //must be called before destory it parent window
    SDL_DestroyRenderer(renderer);
    //destory os-level window resources
    SDL_DestroyWindow(window);
    //destory event queue,input device states
    //can have event become orphaned if it is called first
    SDL_Quit();

    return 0;
}