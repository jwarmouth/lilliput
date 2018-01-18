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
void gnome::setup(string gnomesPath) {
    w = 225; //192;    //256;  // 225
    h = 162; //159;    //212; // 162
    vidWidth = 1920;
    vidHeight = 1080;
    flip = 1;    // 1: normal, -1: flipped
    
    gnomesDirectory = gnomesPath;
    sequence.enableThreadedLoad(true);
//    sequence.setFrameRate(30); // 10 fps for Muybridge horse
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
    //while(isThreadRunning()) {
        if (activeGnome) {
            counter = sequence.getCurrentFrame();
            if (counter > sequence.getCurrentFrame()) {
                activeGnome = false;  // Deactivate if it reaches end of sequence
            }
            
            //sequence.getTextureForTime(ofGetElapsedTimef()).draw(x-w, y-h/2*flip, w, h*flip);
            
        }
    //}
}


//--------------------------------------------------------------
void gnome::draw() {
    //get the frame based on the current time and draw it
    sequence.getTextureForTime(ofGetElapsedTimef()).draw(x-w, y-h/2*flip, w, h * flip);
    
    //sequence.getTextureForTime(ofGetElapsedTimef()).draw(x, y, w, h);
    
    // Draw # of frames of this Gnome
//    ofDrawBitmapStringHighlight(ofToString(sequence.getCurrentFrame()), x, y);
//    
//    string toDraw = ofToString(x) + ", " + ofToString(y);
//    ofDrawBitmapStringHighlight(toDraw, x, y + 10);
//    
//    ofDrawBitmapStringHighlight(ofToString(dx), x, y - 10);
}


//--------------------------------------------------------------
void gnome::loadGnomeSequence() {
    sequence.unloadSequence();
    gnomeDir = chooseRandomGnome();
    cout << "Gnome Directory: " << gnomeDir << endl;
    ofDirectory g(gnomeDir);
    g.listDir();
    numFrames = g.size() - 1;
    string gnomePrefix = gnomeDir + "/gnome_";
    sequence.loadSequence(gnomePrefix, "png", 0, numFrames, 4);
    sequence.preloadAllFrames();	//this way there is no stutter when loading frames
    
    ofFile file (gnomeDir + "/fps.txt");
    string fpsString;
    file >> fpsString;
    float fps = ofToFloat(fpsString);
    if (fps > 30) fps = 30;
    sequence.setFrameRate((int)fps); //set to ten frames per second for Muybridge's horse.
}


//--------------------------------------------------------------
string gnome::chooseRandomGnome() {
    // Randomly choose a gnome from directory
    ofDirectory dir(gnomesDirectory + "/Gnomes");
    dir.listDir();
    return dir.getPath(floor(ofRandom(dir.size())));
}


//--------------------------------------------------------------
void gnome::setRandomPosition() {
    x = w;
    y = (int)(ofRandom(h/2, 1080-(h/2)));
    
    // 50% chance to flip horixontally
    if (ofRandom(2) > 1) {
        flip = -flip;
    }
    
    // Remember that x&y are flipped, since Kinect & screen are tilted sideways!
    // Should probably do a translator for that in the movement or draw script
}


//--------------------------------------------------------------
//void gnome::fall() {
//    
//    if (x < vidWidth - w * 2) {
//        dx += speed;
//        x += dx;
//    }

    
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
    
//}


//--------------------------------------------------------------
//void gnome::calculateGravity() {

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
//}




