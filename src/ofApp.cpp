//         __     ________
//        / /__  / __/ __/_  __
//   __  / / _ \/ /_/ /_/ / / /
//  / /_/ /  __/ __/ __/ /_/ /
//  \____/\___/_/ /_/  \__,_/   LILLIPUT ©2017     */

//  ofApp.cpp
//  Lilliput

#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup(){
    
    defineShaders();
    
    recordingState = PAUSED;
    humanDetected = false;
    gnomeDirectory = "/Jeffu Documents/ART/2017 Lilliput/Saved Gnomes";
    // Would like to read this from a .txt file - even better, to pick a folder.
    
    // Set threadRecorder defaults
    threadRecorder.setPrefix("/gnome_");
    threadRecorder.setFormat("png"); // png is really slow but high res, bmp is fast but big, jpg is just right
    
    w = 1920;
    h = 1080;
    depthW = 1500;
    depthH = 1080;
    saveW = 1024; //256 / 512 / 1024;
    saveH = 848; //212 / 424 / 848;
    
    screenRotation = 90;
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    process_occlusion = false;
    draw_video = true;
    maxFramesPerGnome = 900;
    minFramesPerGnome = 90;
    recordingDelay = 0.5f;
    recordingTimer = 0.0f;
    calibrate = true;

    
    // Initialize Kinect
    kinect0.open(true, true, 0, 2);
    kinect0.start();
    gr.setup(kinect0.getProtonect(), 2);
    
    
    // Loop through and initialize Gnomes
    numGnomes = 5;
    for (int i=0; i<numGnomes; i++) {
        gnomes[i].setup();
        
        // If we use a vector we should use this code
//        gnome tempGnome;
//        tempGnome.setup();
//        gnomes.push_back(tempGnome);
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    detectHuman();
    kinect0.update();
    if (kinect0.isFrameNew()){
        colorTex0.loadData(kinect0.getColorPixelsRef());
        depthTex0.loadData(kinect0.getDepthPixelsRef());
        
        if (draw_ir) {
            irTex0.loadData(kinect0.getIrPixelsRef());
        }
        
//        if (calibrate) {
//            calibrateBackground();
//        }
        
        depthTex0.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST); // or GL_LINEAR
        gr.update(depthTex0, colorTex0, process_occlusion);
        // any chance we can feather the edge and get rid of single outlier pixels?
        
        if (recordingState == RECORDING) {
            checkRecording();
        }
        
        // Wait 1 second before beginning to record Gnome
        else if (recordingState == WAITING && ofGetElapsedTimef() > recordingTimer) {
            startRecording();
        }

        // Loop through & Update Gnomes if active
        for (int i=0; i<numGnomes; i++) {
            if (gnomes[i].activeGnome) {
                gnomes[i].update();
            }
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    ofSetColor(255);
    
    if (colorTex0.isAllocated() && depthTex0.isAllocated()) {
        
    // Draw Kinect video frame
        colorTex0.draw(0, 0, w, h);
        
    // Draw Depth
        if (draw_depth) {
            depthShader.begin();
            depthTex0.draw((w-depthW+50)/2, -50, depthW+10, depthH+100);
            depthShader.end();
        }
        
    // Draw IR
        if (draw_ir) {
            irShader.begin();
            irTex0.draw(210, 0, depthW, depthH);
            irShader.end();
        }
        
    // Draw Registered
        if (draw_registered) {
            gr.getRegisteredTexture(process_occlusion).draw((w-depthW+50)/2, -50, depthW+10, depthH+100);
            // gr.getRegisteredTexture(process_occlusion).draw(0, 0, w, h);
        }
        
    // Loop through & Draw Gnomes
        for (int i=0; i<numGnomes; i++) {
            if (gnomes[i].activeGnome) {
                gnomes[i].draw();
            }
        }
        
    // Draw Recording Icon
        if (recordingState == RECORDING) {
            // Draw Recorded frame
            // frameFbo.draw(0, 0, w, h);
            
            ofSetColor(255, 0, 0);
            ofDrawCircle(30, 50, 15);
        }
        
        // Try to simply use depthTex0 as an alpha for colorTex0. Like... draw it onto a 1920x1080 rect? NO, this will not work.
    }
    
    // Draw Frame Rate to screen
        ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
    

}


//--------------------------------------------------------------
void ofApp::detectHuman(){
    
    if (humanDetected && recordingState == PAUSED) {
        recordingState = WAITING;
        recordingTimer = recordingDelay + ofGetElapsedTimef();
            // Set up a timer - start recording after 15-30 frames
    }
    
    if (!humanDetected) {
        if (recordingState == WAITING) {
            recordingState = PAUSED;
        }
        if (recordingState == RECORDING) {
            stopRecording();
        }
    }
}


//--------------------------------------------------------------
void ofApp::startRecording(){
    recordingState = RECORDING;
    frameCount = 0;
    makeNewDirectory();
    
//    for (int i=0; i<numGnomes; i++) {
//        if (!gnomes[i].activeGnome) {
//            gnomes[i].setup();
//            return;
//        }
//    }
//    theGnome.chooseRandomGnome();
}


//--------------------------------------------------------------
void ofApp::makeNewDirectory(){
    currentPath = gnomeDirectory + "/gnome_" + ofGetTimestampString();
    ofDirectory dir(currentPath);
    dir.createDirectory(currentPath);
    
    // IT WORKS!!!  http://openframeworks.cc/documentation/utils/ofDirectory/
    
    //recorder.setPrefix(ofToDataPath("recording1/frame_")); // this directory must already exist
    
    threadRecorder.setPath(currentPath); // this directory must already exist
    threadRecorder.setCounter(0);
    
    if(!threadRecorder.isThreadRunning()){
        threadRecorder.startThread();
        //        threadRecorder.startThread(false, true);
    }
}


//--------------------------------------------------------------
void ofApp::saveFrame(){
    
    // Draw the depth texture into a Frame Buffer Object
    ofFbo depthFbo;
    depthFbo.allocate(saveW, saveH, GL_RGBA);
    depthFbo.begin();
    ofClear(0, 0, 0, 0);    // clear fbo
    depthShader.begin();
    depthTex0.draw(0, 0, saveW, saveH);
    depthShader.end();
    depthFbo.end();
    
    // Draw the Registered video texture into a Frame Buffer Object
    //    ofFbo fbo;
    //    fbo.allocate(saveW, saveH, GL_RGBA);
    //    fbo.begin();	//Start drawing into buffer
    //    ofClear(0, 0, 0, 0);    // clear fbo
    ////    float vidW = saveH * w / h;
    ////    float vidH = saveH * 1.05;
    ////    colorTex0.draw((vidW - saveW) / -2 , (vidH - saveH) / -2, vidW, vidH);
    //    // Draw Registered Texture, use Depth as alpha
    //    gr.getRegisteredTexture(process_occlusion).draw(0, 0, saveW, saveH);
    //    fbo.end();
    //   fbo.getTexture().setAlphaMask(depthFbo.getTexture());
    // Set the alpha mask -- no longer needed since we are using a shader
    
    
    // Draw Registered Texture into new FBO using Depth as alpha shader
    frameFbo.allocate(saveW, saveH, GL_RGBA);
    frameFbo.begin();
    ofClear(0, 0, 0, 0);
    alphaShader.begin();    // pass depth fbo to the alpha shader
    alphaShader.setUniformTexture("maskTex", depthFbo.getTexture(), 1 );
    gr.getRegisteredTexture(process_occlusion).draw(0, 0, saveW, saveH);
    alphaShader.end();
    frameFbo.end();
    
    
    // Prepare pixels object
    ofPixels pix; // allocate pix
    frameFbo.readToPixels(pix);
    threadRecorder.addFrame(pix);
    //    fileName = currentPath + "/gnome_" + ofToString(frameCount, 3, '0') + ".png";
    //    ofSaveImage(pix, fileName, OF_IMAGE_QUALITY_BEST);
    
    //  IT WORKS!!! Saves .png sequence with alpha channel
    //  how to draw into fbo and save fbo to png file....
    //  https://forum.openframeworks.cc/t/convert-int-to-char/1632/7
    //  https://forum.openframeworks.cc/t/saving-frames-with-transparent-background/26363/7
    //  http://openframeworks.cc/documentation/gl/ofFbo/
}


//--------------------------------------------------------------
void ofApp::checkRecording(){
    if (frameCount > maxFramesPerGnome) {
        stopRecording();
        // HMMM - what to do if someone wants to stay & play longer?
    }
    saveFrame();
    frameCount ++;
}


//--------------------------------------------------------------
void ofApp::stopRecording(){
    
    recordingState = PAUSED;
    threadRecorder.addFrame();  // add a blank frame to trigger closeFolder
    
    
    
//    threadRecorder.waitForThread();
    
    
    // Folder closing is now on threadRecorder
    // as it is prematurely clamping the folder before anything is added
    // SO, either set it in a single thread that can detect when the folder changes
    // OR initiate a new thread for each Gnome clip - maybe 5 that can be cycled through?
    
    // TELL THE THREAD TO DO THIS - IN QUEUE
    // SEND IT A BLACK IMAGE?
    // ALLOW IT TO KNOW THE CURRENT PATH
    
//    // Set current recording path
//    ofDirectory dir(currentPath);
//    dir.listDir();
//    int size = dir.size();
//    
//    if (size < minFramesPerGnome) {
//        // Delete directory if fewer than 3 seconds / 90 frames
//        dir.remove(true);
//    } else {
//        // Clean up the last 15 frames - they will be garbage
//        for (int i = size - 15; i < size; i++) {
//            dir.getFile(i).remove();
//        }
//    }
    
    // fileName = currentPath + "/gnome_" + ofToString(frameCount, 3, '0') + ".png";
}


//--------------------------------------------------------------
void ofApp::defineShaders(){
    
    // Define Shaders - thanks to Yuya Hanai
    #ifdef TARGET_OPENGLES
        alphaShader.load("shadersES2/shader");
    
    #else
        if(ofIsGLProgrammableRenderer()){
            alphaShader.load("shadersGL3/shader");
        }else{
            alphaShader.load("shadersGL2/shader");
        }
    #endif
    
    depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, depthFragmentShader);
    depthShader.linkProgram();
    
    irShader.setupShaderFromSource(GL_FRAGMENT_SHADER, irFragmentShader);
    irShader.linkProgram();
}


//--------------------------------------------------------------
void ofApp::calibrateBackground(){
    calibrate = false;
}


//--------------------------------------------------------------
void ofApp::calculateAlpha(){
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        humanDetected = true;
    }
    if (key =='r') {
        draw_registered = !draw_registered;
    }
    
    if (key == 'd') {
        draw_depth = !draw_depth;
    }
    
    if (key == 'o') {
        process_occlusion = !process_occlusion;
    }
    
    if (key == 'v') {
        draw_video = !draw_video;
    }
    
    if (key == 'i') {
        draw_ir = !draw_ir;
    }
    
    if (key == 'c') {
        calibrate = true;
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' ') {
        humanDetected = false;
    }
    // This will fail if a viewer jumps - or if they aren't actually on the mat
}


//--------------------------------------------------------------
void ofApp::exit() {
    threadRecorder.waitForThread();
}


