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
    
    // Shaders
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
    
    
    isRecording = false;
    humanDetected = false;
    gnomeDirectory = "/Jeffu Documents/ART/2017 Lilliput/Saved Gnomes";
    // Would like to read this from a .txt file - even better, to pick a folder.
    
    w = 1920;
    h = 1080;
    depthW = 1500;
    depthH = 1080;
    screenRotation = 90;
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    process_occlusion = false;
    draw_video = true;
    maxFramesPerGnome = 900;

    
    // Kinect
    kinect0.open(true, true, 0, 2);
    kinect0.start();
    gr.setup(kinect0.getProtonect(), 2);
    
    // initialize gnomes
    //    numGnomes = 5;
    //    for (int i=0; i<numGnomes; i++) {
    //        gnomes[i].setup();
    //    }

}

//--------------------------------------------------------------
void ofApp::update(){
    detectHuman();
    kinect0.update();
    if (kinect0.isFrameNew()){
        colorTex0.loadData(kinect0.getColorPixelsRef());
        depthTex0.loadData(kinect0.getDepthPixelsRef());
        
        depthTex0.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST); // or GL_LINEAR
        gr.update(depthTex0, colorTex0, process_occlusion);
        // any chance we can feather the edge and get rid of single outlier pixels?
        
        if (isRecording) {
            checkRecording();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    ofSetColor(255, 255, 255);
    
    if (colorTex0.isAllocated() && depthTex0.isAllocated()) {
        
        if (isRecording) {
            frameFbo.draw(0, 0, w, h);
        }
        
        // Try to simply use depthTex0 as an alpha for colorTex0. Like... draw it onto a 1920x1080 rect? NO, this will not work.
        else if (draw_video) {
            colorTex0.draw(0, 0, w, h);
        }
        
        if (draw_registered) {
            gr.getRegisteredTexture(process_occlusion).draw((w-depthW+50)/2, -50, depthW+10, depthH+100);
//            gr.getRegisteredTexture(process_occlusion).draw(0, 0, w, h);
        }
        
        if (draw_depth) {
            depthShader.begin();
            depthTex0.draw((w-depthW+50)/2, -50, depthW+10, depthH+100);
            depthShader.end();
        }
    }
    
    ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 10, 20);
    
    //  Loop through & display Gnomes
//    for (int i=0; i<numGnomes; i++) {
//        gnomes[i].draw();
//    }

}


//--------------------------------------------------------------
void ofApp::detectHuman(){
    if (humanDetected && !isRecording) {
        startRecording();
    } else if (!humanDetected && isRecording) {
        stopRecording();
    }
}

//--------------------------------------------------------------
void ofApp::startRecording(){
    isRecording = true;
    frameCount = 0;
    makeNewDirectory();
    // Might set up a timer - start recording after 15-30 frames
}

//--------------------------------------------------------------
void ofApp::stopRecording(){
    isRecording = false;
    // Clean up the last 15-20 frames - they will be garbage
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
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' ') {
        humanDetected = false;
    }
    // This will fail if a viewer jumps - or if they aren't actually on the mat
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
void ofApp::saveFrame(){
    
    int saveW = 1024; //512;
    int saveH = 848; //424;
    
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
    alphaShader.begin();
    // pass depth fbo to the alpha shader
    alphaShader.setUniformTexture("maskTex", depthFbo.getTexture(), 1 );
    gr.getRegisteredTexture(process_occlusion).draw(0, 0, saveW, saveH);
    alphaShader.end();
    frameFbo.end();
    
    // Prepare pixels object
    ofPixels pix; // allocate pix
    frameFbo.readToPixels(pix);
    fileName = currentPath + "/gnome_" + ofToString(frameCount) + ".png";
    ofSaveImage(pix, fileName, OF_IMAGE_QUALITY_BEST);
    
    // IT WORKS!!! Saves .png sequence with alpha channel

//    how to draw into fbo and save fbo to png file....
//    https://forum.openframeworks.cc/t/convert-int-to-char/1632/7
//    https://forum.openframeworks.cc/t/saving-frames-with-transparent-background/26363/7
//    http://openframeworks.cc/documentation/gl/ofFbo/
}

//--------------------------------------------------------------

void ofApp::makeNewDirectory(){
    currentPath = gnomeDirectory + "/gnome_" + ofGetTimestampString();
    ofDirectory dir(currentPath);
    dir.createDirectory(currentPath);
    // IT WORKS!!!
    
    //    create a unique directory based on system time stamp
    //    http://openframeworks.cc/documentation/utils/ofDirectory/
    
    //    string path = "/my/path/file";    // path may be absolute or relative to bin/data
    //    ofDirectory dir(path);
    //    dir.allowExt("png");  //only show png files
    //    dir.listDir();    //populate the directory object
    //    for(int i = 0; i < dir.size(); i++){  //loop through and print out all the paths
    //        ofLogNotice(dir.getPath(i));
    //    }
}

//--------------------------------------------------------------
void ofApp::calculateAlpha(){
    
}

