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

typedef enum {
    PAUSED,
    WAITING,
    RECORDING,
    WAITING_TO_STOP
} RecordingState;

class ofApp : public ofBaseApp{

public:
    bool isRecording, humanDetected, isWaitingToRecord;
    string gnomeDirectory, currentPath, fileName;
    int frameCount, maxFramesPerGnome, minFramesPerGnome;
    float recordingDelay, recordingTimer, gnomeInterval, gnomeTimer;
    
    ofxImageSequenceRecorder threadRecorder;
    RecordingState recordingState;
    
    ofxMultiKinectV2 kinect0;
    GpuRegistration gr;
    
    ofTexture colorTex0, depthTex0, irTex0;
    ofShader depthShader, irShader, alphaShader, shaderBlurX, shaderBlurY;
    ofFbo frameFbo, depthFbo, fboBlurOnePass, fboBlurTwoPass;
    
    bool process_occlusion, draw_depth, draw_registered, draw_ir, draw_video, calibrate;
    
    //  Width & Height of Video
    int w, h, depthH, depthW, saveW, saveH;
    float screenRotation;
    
    //  Kinect & OpenCV Variables
    //    ofxMultiKinectV2 kinect;
    //    ofxCvColorImage colorImg;
    //    ofImage rgbaImage, depthImage, drawImage;
    //    ofxCvGrayscaleImage grayImage, blurAlpha;
    
    int nearThreshold, farThreshold;
    bool learnBkd;
    //    ofxCvContourFinder contourFinder;
    
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
    void calculateAlpha();
    void checkRecording();
    void checkKeys();
    void calibrateBackground();
    void defineShaders();
    void activateGnome();
    
    
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
              float low1 = 1000.0; // near distance in cm
              float high1 = 3000.0; // far distance in cm
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
              float value = col.r / 1000.0;
              gl_FragColor = vec4(vec3(value), 1.0);
          }
          );


/*
static string blurFragmentShaderX =
STRINGIFY(
          uniform sampler2DRect tex;
          uniform float blurAmount = 2.0;
          in vec2 textCoordVarying;
          out vec4 outputColor;
          
          void main()
          {
              vec4 color;
              color += 1.0 * texture(tex0, texCoordVarying + vec2(blurAmount * -4.0, 0.0));
              color += 2.0 * texture(tex0, texCoordVarying + vec2(blurAmount * -3.0, 0.0));
              color += 3.0 * texture(tex0, texCoordVarying + vec2(blurAmount * -2.0, 0.0));
              color += 4.0 * texture(tex0, texCoordVarying + vec2(blurAmount * -1.0, 0.0));
              
              color += 5.0 * texture(tex0, texCoordVarying + vec2(blurAmount, 0));
              
              color += 4.0 * texture(tex0, texCoordVarying + vec2(blurAmount * 1.0, 0.0));
              color += 3.0 * texture(tex0, texCoordVarying + vec2(blurAmount * 2.0, 0.0));
              color += 2.0 * texture(tex0, texCoordVarying + vec2(blurAmount * 3.0, 0.0));
              color += 1.0 * texture(tex0, texCoordVarying + vec2(blurAmount * 4.0, 0.0));
              
              color /= 25.0;
              
              outputColor = color;
              
          }
          );

static string blurFragmentShaderY =
STRINGIFY(
          uniform sampler2DRect tex;
          uniform float blurAmount;
          in vec2 textCoordVarying;
          out vec4 outputColor;
          
          void main()
          {
              vec4 color;
              color += 1.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * -4.0));
              color += 2.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * -3.0));
              color += 3.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * -2.0));
              color += 4.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * -1.0));
              
              color += 5.0 * texture(tex0, texCoordVarying + vec2(0, blurAmount));
              
              color += 4.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * 1.0));
              color += 3.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * 2.0));
              color += 2.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * 3.0));
              color += 1.0 * texture(tex0, texCoordVarying + vec2(0.0, blurAmount * 4.0));
              
              color /= 25.0;
              
              outputColor = color;
              
          }
          );

*/
