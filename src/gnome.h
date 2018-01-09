//         __     ________
//        / /__  / __/ __/_  __
//   __  / / _ \/ /_/ /_/ / / /
//  / /_/ /  __/ __/ __/ /_/ /
//  \____/\___/_/ /_/  \__,_/   LILLIPUT ©2017     */
//
//  gnome.h
//  Lilliput

#ifndef gnome_h
#define gnome_h


#endif /* gnome_h */


#pragma once

#include "ofMain.h"
#include "ofxMultiKinectV2.h"
#include "ofxOpenCv.h"
#include "ofxImageSequence.h"

class gnome {
public:
    
    // Methods
    void setup();
    void reset();
    void update();
    void draw();
//    void fall();
    string chooseRandomGnome();
    void loadGnomeSequence();
    void setRandomPosition();
//    void calculateGravity();
    
    // Properties
    float x, y, dx, dy, vidWidth, vidHeight, speed;
    int w, h, numFrames, counter, flipped;
    bool activeGnome;
    string gnomesDirectory, gnomeDir;
    ofxImageSequence sequence;
    
//    ofVideoPlayer vid;
//    bool playing;
    
    // References to "Global" Properties
//    float terminalVelocity;
//    ofPixels pix;
    
//    ball(); // constructor
//    http://openframeworks.cc/ofBook/chapters/OOPs!.html 
};
