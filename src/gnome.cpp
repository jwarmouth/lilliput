//         __     ________
//        / /__  / __/ __/_  __
//   __  / / _ \/ /_/ /_/ / / /
//  / /_/ /  __/ __/ __/ /_/ /
//  \____/\___/_/ /_/  \__,_/   LILLIPUT ©2017     */
//
//  gnome.cpp
//  Lilliput

#include "gnome.h"



//--------------------------------------------------------------
void gnome::setup() {
    
    w = 256;
    h = 212;
    vidWidth = 1920;
    vidHeight = 1080;
    speed = 5.0;
    flipped = 1;    // 1: normal, -1: flipped
    
    gnomesDirectory = "/Jeffu Documents/ART/2017 Lilliput/Saved Gnomes/";
    sequence.enableThreadedLoad(true);
    sequence.setFrameRate(30); //set to ten frames per second for Muybridge's horse.

}


//--------------------------------------------------------------
void gnome::reset() {
    
    activeGnome = true;
    counter = 0;
    dx = 0;
    loadGnomeSequence();
    setRandomPosition();
}


//--------------------------------------------------------------
void gnome::update() {
    if (counter > sequence.getCurrentFrame()) {
        // Deactivate if it reaches end of sequence
//        activeGnome = false;
    } else {
        counter = sequence.getCurrentFrame();
    }
    
//    fall();
    
}


//--------------------------------------------------------------
void gnome::draw() {
    //get the frame based on the current time and draw it
    sequence.getTextureForTime(ofGetElapsedTimef()).draw(x - w, y - h / 2 * flipped, w, h * flipped);
    
    // Draw # of frames of this Gnome
    ofDrawBitmapStringHighlight(ofToString(sequence.getCurrentFrame()), x, y);
    
    string toDraw = ofToString(x) + ", " + ofToString(y);
    ofDrawBitmapStringHighlight(toDraw, x, y + 10);
    
    ofDrawBitmapStringHighlight(ofToString(dx), x, y - 10);
}


//--------------------------------------------------------------
void gnome::loadGnomeSequence() {
    gnomeDir = chooseRandomGnome();
    ofDirectory g(gnomeDir);
    g.listDir();
    numFrames = g.size() - 1;
    string gnomePrefix = gnomeDir + "/gnome_";
    sequence.loadSequence(gnomePrefix, "png", 0, numFrames, 4);
    sequence.preloadAllFrames();	//this way there is no stutter when loading frames
}


//--------------------------------------------------------------
string gnome::chooseRandomGnome() {
    // Randomly choose a gnome from directory
    ofDirectory dir(gnomesDirectory);
    dir.listDir();
    return dir.getPath(floor(ofRandom(dir.size())));
}


//--------------------------------------------------------------
void gnome::setRandomPosition() {
    x = 0;
    y = (int)(ofRandom(h/2, 1080-(h/2)));
    
    // 50% chance to flip horixontally
    if (ofRandom(2) > 1) {
        flipped = -flipped;
//        h *= -1;
    }
    
    // Remember that x&y are flipped, since Kinect & screen are tilted sideways!
    // Should probably do a translator for that in the movement or draw script
}


//--------------------------------------------------------------
void gnome::fall() {
    
    if (x < vidWidth - w * 2) {
        dx += speed;
        x += dx;
    }
    
    
    //  If gnome falls to bottom, Reset Gnome
//    if (x > vidWidth - h) {
//        activeGnome = false;
//    }
    
    
    //    //  Fall Down
    //    if (speed <= terminalVelocity) {
    //        speed ++;
    //    }
    //
    //    y += speed;
    //
    //    //  If gnome falls to bottom, Reset Gnome
    //    if (y > vidWidth - h) {
    //        setup();
    //    }
    
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




