//         __     ________
//        / /__  / __/ __/_  __
//   __  / / _ \/ /_/ /_/ / / /
//  / /_/ /  __/ __/ __/ /_/ /
//  \____/\___/_/ /_/  \__,_/   LILLIPUT ©2017     */

//  ofApp.h
//  Lilliput

#pragma once

#include "ofMain.h"
#include "ofxMultiKinectV2.h"
#include "ofxOpenCv.h"
#include "gnome.h"
#include "GpuRegistration.h"
#include "ofxImageSequence.h"
#include "ofxImageSequenceRecorder.h"
#include <fstream>

typedef enum {
    PAUSED,
    WAITING,
    RECORDING,
    WAITING_TO_STOP
} RecordingState;

class ofApp : public ofBaseApp{

public:
    bool isRecording, humanDetected, isWaitingToRecord;
    string gnomesDirectory, currentPath, fileName;
    int frameCount, maxFramesPerGnome, minFramesPerGnome, threshold, offset;
    float recordingDelay, recordingTimer, gnomeInterval, gnomeTimer, gravity, bgCheckTimer;
    
    ofxImageSequenceRecorder threadRecorder;
    RecordingState recordingState;
    
    ofxMultiKinectV2 kinect0;
    GpuRegistration gr;
    
    // Background Learning for contours
    ofxCvColorImage	colorImg;
    ofxCvGrayscaleImage grayImage, grayBg, grayDiff, grayBgOld;
    ofxCvContourFinder contourFinder;
    
    
    ofTexture colorTex0, depthTex0, irTex0;
    ofShader depthShader, irShader, alphaShader, shaderBlurX, shaderBlurY;
    ofFbo frameFbo, depthFbo, depthFullFbo, depthCropFbo, irFbo, saveFbo, grayFbo, fullFrameFbo, fboBlurOnePass, fboBlurTwoPass, guiFbo;
    
    bool process_occlusion, calibrate;
    bool draw_depth, draw_registered, draw_ir, draw_video, draw_blur, draw_gray, draw_contours, draw_alpha, draw_gui;
    
    //  Width & Height of Video
    int w, h, depthH, depthW, frameW, frameH, saveW, saveH;
    float screenRotation;
    
    // vec4 depthDraw;
    
    //  Kinect & OpenCV Variables
    //    ofxMultiKinectV2 kinect;
    //    ofxCvColorImage colorImg;
    //    ofImage rgbaImage, depthImage, drawImage;
    //    ofxCvGrayscaleImage grayImage, blurAlpha;
    
    int nearThreshold, farThreshold;
    bool learnBkd;
    
    //     The Gnomes
    int numGnomes;
    gnome gnomes[5];
//    gnome theGnome;
//    vector <gnome> gnomes;

    
    // Jeffu methods
    void detectHuman();
    void startRecording();
    void stopRecording();
    void waitToStartRecording();
    void waitToStopRecording();
    void saveFrame();
    void makeNewDirectory();
    void blurDepth();
    void calculateAlpha();
    void checkRecording();
    void checkKeys();
    void calibrateBackground();
    void defineShaders();
    void activateGnome();
    void calculatePhysics();
    void drawGui();
    void backgroundCheck();
    
    
    // openFrameworks methods
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void exit();
		
};

#define STRINGIFY(x) #x

static string depthFragmentShader =
STRINGIFY(
          uniform sampler2DRect tex;
          void main()
          {
              vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
              float value = col.r;
              float low1 = 1500.0; // near distance in cm
              float high1 = 2800.0; // far distance in cm
              float low2 = 1.0;
              float high2 = 0.0;
              float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
              if (d == 1.0) {
                  d = 0.0;
              }
              if (d > 0.5) {
                  d = 1.0;
              } else {
                  d = 0.0;
              }
              gl_FragColor = vec4(vec3(d), 1.0);
          }
          );


static string irFragmentShader =
STRINGIFY(
          uniform sampler2DRect tex;
          void main()
          {
              vec4 col = texture2DRect(tex, gl_TexCoord[0].xy);
//              float value = col.r / 65535.0;
              float value = col.r / 2000.0;
              gl_FragColor = vec4(vec3(value), 1.0);
          }
          );

