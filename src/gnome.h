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
#include <fstream>

class gnome: public ofThread {
public:
    
    // Methods
    void setup(string gnomesPath);
    void reset();
    void update();
    void draw();
//    void fall();
    string chooseRandomGnome();
    void loadGnomeSequence();
    void setRandomPosition();
//    void calculateGravity();
    
    void threadedFunction();
    
    // Properties
    float x, y, dx, dy, vidWidth, vidHeight, speed;
    int w, h, numFrames, counter, flip;
    bool activeGnome;
    string gnomesDirectory, gnomeDir;
    ofxImageSequence sequence;
    
    // References to "Global" Properties
//    float terminalVelocity;
//    ofPixels pix;
    
//    ball(); // constructor
//    http://openframeworks.cc/ofBook/chapters/OOPs!.html 
};
