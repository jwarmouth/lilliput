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
    void fall();
    void randomGnome();
    void gravity();
    
    // Properties
    float x;
    float y;
    int w;
    int h;
    float speed;
    string gnomesDirectory;
    string gnome;
    ofxImageSequence sequence;
    
//    ofVideoPlayer vid;
//    bool playing;
    
    // References to "Global" Properties
    int vidWidth;
    int vidHeight;
    float terminalVelocity;
    ofPixels pix;
};
