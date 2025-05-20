# Piano Learning Application
A C++ application designed to help beginners learn piano faster through visualization and interactive practice.

# Overview
The app uses sdl_mixer, SDL and ImGui to help create user friendly app. It has feature such as scale and chord visualization, interactive piano keyboard and note names on keys to help beginners. 

# Technical Implementation

## Piano Module: 
- Handles keyboard rendering 
- sound playback
- input
## Music Theory Module: 
- Manages scales and chords
- highlighting logic
## Main Module: 
- coordinates the application flow 
- SDL initialization

# Getting Started
## Prerequisites

- C++ compiler 
- SDL2 and SDL2_mixer libraries
- ImGui library

# Installation

## Clone the repository

git clone https://github.com/Blackwise123/Piano-Imgui

## Build using CMake

mkdir build
cd build
cmake ..
make

## Run the application

./Piano

username:s
password:s

# Screenshots
![image](https://github.com/user-attachments/assets/89808b90-9cda-40eb-b14f-af69c74927b9)

![image](https://github.com/user-attachments/assets/4d46c506-b855-4058-9298-b470deaca809)


# Future Development
- Show music sheet
- Create reaction game where one read and press correct random keys
- Make the code maintainable such have header file for login screen and piano
- Have config.ini that change piano layout and keybinding

# Acknowlegdments 

- Piano sound files from piano-mp3 [repository](https://github.com/fuhton/piano-mp3)
- ImGui library by Omar Cornut
- SDL2 and related libraries
