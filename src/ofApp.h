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
#include "ofxVideoRecorder.h"
#include "gnome.h"
#include "GpuRegistration.h"
#include "ofxImageSequence.h"

typedef enum {
    PAUSED,
    WAITING,
    RECORDING
} RecordingState;

class ofApp : public ofBaseApp{

public:
    RecordingState recordingState;
    bool isRecording;
    bool humanDetected;
    bool isWaitingToRecord;
    string gnomeDirectory;
    string currentPath;
    string fileName;
    int frameCount;
    int maxFramesPerGnome;
    int minFramesPerGnome;
    float recordingDelay;
    float recordingTimer;
//    ofFile saveLocation;
    
    ofxMultiKinectV2 kinect0;
    
    ofTexture colorTex0;
    ofTexture depthTex0;
    ofTexture irTex0;
    
    GpuRegistration gr;
    
    ofShader depthShader;
    ofShader irShader;
    ofShader alphaShader;
    ofFbo frameFbo;
    
    bool process_occlusion;
    bool draw_depth;
    bool draw_registered;
    bool draw_ir;
    bool draw_video;
    bool calibrate;

    
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
    
    //    Video Recorder
//    ofPtr<ofQTKitGrabber> vidRecorder;
//    void videoSaved(ofVideoSavedEventArgs& e);
//    vector<string> videoDevices;
    
    // Jeffu methods
    void detectHuman();
    void startRecording();
    void stopRecording();
    void saveFrame();
    void makeNewDirectory();
    void calculateAlpha();
    void checkRecording();
    void checkKeys();
    void calibrateBackground();
    void defineShaders();
    
    
    // openFrameworks methods
    void setup();
    void update();
    void draw();
    
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    //		void mouseMoved(int x, int y );
    //		void mouseDragged(int x, int y, int button);
    //		void mousePressed(int x, int y, int button);
    //		void mouseReleased(int x, int y, int button);
    //		void mouseEntered(int x, int y);
    //		void mouseExited(int x, int y);
    //		void windowResized(int w, int h);
    //		void dragEvent(ofDragInfo dragInfo);
    //		void gotMessage(ofMessage msg);
		
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
              float value = col.r / 65535.0;
              gl_FragColor = vec4(vec3(value), 1.0);
          }
          );
