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
    
    activeGnome = true;
    
    terminalVelocity = 20;
    vidWidth = 640;
    vidHeight = 480;
    w = 256;
    h = 212;
    
    gnomesDirectory = "/Jeffu Documents/ART/2017 Lilliput/Saved Gnomes/";
//    sequence.loadSequence("frame", "png", 1, 11, 2);
    
    loadGnomeSequence();
    setRandomPosition();
    
}

//--------------------------------------------------------------
void gnome::update() {
    
    calculateGravity();
    
}

//--------------------------------------------------------------
void gnome::draw() {
    
    //get the frame based on the current time and draw it
    sequence.getFrameForTime(ofGetElapsedTimef())->draw(x,y, w, h);
    
    ofDrawBitmapStringHighlight(ofToString(numFrames), x, y);
//    ofDrawBitmapStringHighlight(ofToString(ofGetElapsedTimef()), x, y);
    
    
//    vid.draw(x*2, y*2, w*2, h*2);
}

//--------------------------------------------------------------
void gnome::loadGnomeSequence() {
    gnomeDir = chooseRandomGnome();
    ofDirectory g(gnomeDir);
    g.listDir();
    numFrames = g.size();
    sequence.enableThreadedLoad(true);
//    sequence.loadSequence(gnomeDir);
    string gnomePrefix = gnomeDir + "/gnome_";
    sequence.loadSequence(gnomePrefix, "png", 0, numFrames, 3);
    sequence.preloadAllFrames();	//this way there is no stutter when loading frames
    sequence.setFrameRate(30); //set to ten frames per second for Muybridge's horse.
}


//--------------------------------------------------------------
string gnome::chooseRandomGnome() {
    // Randomly choose a gnome from directory
    ofDirectory dir(gnomesDirectory);
    dir.listDir();
    return dir.getPath(floor(ofRandom(dir.size())));
    
//    sequence.loadSequence(dir.getPath(floor(ofRandom(dir.size()))));
//    vid.load(dir.getPath(floor(ofRandom(dir.size()))));
//    vid.setLoopState(OF_LOOP_NONE);
//    vid.play();
}

//--------------------------------------------------------------
void gnome::setRandomPosition() {
    // Remember that x=y and y=x
    // Should probably do a translator for that in the movement or draw script
    
    //    y = -h;
    //    x = ofRandom(140, 500);
    
    // This SHOULD be the starting position:
//    x = -w;
    
    x = 200;
    y = ofRandom(h/2, 1080-(h/2));
    speed = 0;
    
    // 50% chance to flip horixontally
    // OH - we actually need to use h, since everything is tilted sideways!
    //    if (ofRandom(2)>1) { w *= -1;}
}


//--------------------------------------------------------------
void gnome::calculateGravity() {
    
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

