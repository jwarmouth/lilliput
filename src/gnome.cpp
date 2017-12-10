//         __     ________
//        / /__  / __/ __/_  __
//   __  / / _ \/ /_/ /_/ / / /
//  / /_/ /  __/ __/ __/ /_/ /
//  \____/\___/_/ /_/  \__,_/   LILLIPUT ©2017     */
//
//  gnome.cpp
//  Lilliput
//
//  Created by Jeffrey Warmouth on 12/9/17.
//
//

#include "gnome.h"

//--------------------------------------------------------------

void gnome::setup() {
    
    gnomesDirectory = "/Jeffu Documents/ART/2017 Lilliput/Saved Gnomes/";
//    sequence.loadSequence("frame", "png", 1, 11, 2);
    randomGnome();
    sequence.preloadAllFrames();	//this way there is no stutter when loading frames
    sequence.setFrameRate(30); //set to ten frames per second for Muybridge's horse.
    
    terminalVelocity = 20;
    vidWidth = 640;
    vidHeight = 480;
    
    w = 64;
    h = 64;
    y = -h;
    x = ofRandom(140, 500);
    speed = 0;
    
    // 50% chance to flip horixontally
    if (ofRandom(2)>1) { w *= -1;}
    
}

//--------------------------------------------------------------
void gnome::update() {
    
    
//    //      Gravity & Collision System - calculate bottom of gnome
//    int bottom = vidWidth * (y + h) + x + w/2;
//    
//    //      If bottom depth pixel is white, move upward for each white pixel above bottom
//    //    ofPixels & pix = grayImage.getPixels();
//    if (pix[bottom] == 255) {
//        speed = 0;
//        for (int i=1; i<h; i++) {
//            if (pix[bottom - vidWidth * i] == 0) {
//                break;
//            }
//            speed --;
//        }
//        y += speed;
//        speed = 0;
//        
//    } else {
//        fall();
//    }
}

//--------------------------------------------------------------
void gnome::draw() {
    
    //get the frame based on the current time and draw it
    sequence.getFrameForTime(ofGetElapsedTimef())->draw(640,480);
    
//    vid.draw(x*2, y*2, w*2, h*2);
}


//--------------------------------------------------------------
void gnome::randomGnome() {
    // Randomly choose a gnome from directory
    ofDirectory dir(gnomesDirectory);
    dir.listDir();
    gnome = dir.getPath(floor(ofRandom(dir.size())));
    ofDirectory g(gnome);
    g.listDir();
    sequence.loadSequence(gnome + "/gnome_", "png", 1, g.size(), 3);
    
//    sequence.loadSequence(dir.getPath(floor(ofRandom(dir.size()))));
//    vid.load(dir.getPath(floor(ofRandom(dir.size()))));
//    vid.setLoopState(OF_LOOP_NONE);
//    vid.play();
}


//--------------------------------------------------------------
void gnome::fall() {
    
    //  Fall Down
    if (speed <= terminalVelocity) {
        speed ++;
    }
    
    y += speed;
    
    //  If gnome falls to bottom, Reset Gnome
    if (y > vidWidth - h) {
        setup();
    }
    
}

