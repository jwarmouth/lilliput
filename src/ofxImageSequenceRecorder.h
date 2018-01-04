/*
 
 Based on code by Memo from this thread:
 http://forum.openframeworks.cc/index.php?topic=1687.0
 
 */

//         __     ________
//        / /__  / __/ __/_  __
//   __  / / _ \/ /_/ /_/ / / /
//  / /_/ /  __/ __/ __/ /_/ /
//  \____/\___/_/ /_/  \__,_/   LILLIPUT ©2017     */#include "ofMain.h"

//  ofxImageSequenceRecorder.h
//  Lilliput

#include "ofMain.h"
#include "ofApp.h"

typedef struct {
    string fileName;
    ofPixels image;
} QueuedImage;

class ofxImageSequenceRecorder : public ofThread {
public:
    
    int counter;
    queue<QueuedImage> q;
    string path;
    string prefix;
    string format;
    int numberWidth;
    
    ofxImageSequenceRecorder(){
        counter=0;
        numberWidth=4;
    }
    
    void setPath(string filePath){
        path = filePath;
    }
    
    void setPrefix(string pre){
        prefix = pre;
    }
    
    void setFormat(string fmt){
        format = fmt;
    }
    
    void setCounter(int count){
        counter = count;
    }
    
    void setNumberWidth(int nbwidth){
        numberWidth = nbwidth;
    }
    
    void threadedFunction() {
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage i = q.front();
                if (i.image.size() == 1) {
                    closeFolder();
                } else {
                    ofSaveImage(i.image, i.fileName);
                }
                q.pop();
            }
        }
    }

    
    
    void addFrame(ofPixels imageToSave) {
        
        //char fileName[100];
        //snprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());
        string fileName = path + prefix + ofToString(counter, numberWidth, '0') + "." + format;
        counter++;
        
        QueuedImage qImage;
        
        qImage.fileName = fileName;
        qImage.image = imageToSave;
        
        q.push(qImage);
        
    }
    
    void addFrame() {
        
        // add a blank frame to trigger closeFolder
        QueuedImage qImage;
        
        qImage.fileName = path;
        
        ofPixels dummy;
        dummy.allocate(1, 1, OF_IMAGE_GRAYSCALE);
        qImage.image = dummy;
        
        q.push(qImage);
        
    }
    
    void closeFolder() {
        
        // Set current recording path
        ofDirectory dir(path);
        dir.listDir();
        int size = dir.size();
        
        if (size < 90) {
            // Delete directory if fewer than 3 seconds / 90 frames
            dir.remove(true);
        } else {
            // Clean up the last 15 frames - they will be garbage
            for (int i = size - 15; i < size; i++) {
                dir.getFile(i).remove();
            }
        }
    }
    
    
    
    
    //    void addFrame(ofImage &img){
    //        addFrame(img.getPixels());
    //    }
    //
    //    void addFrame(ofVideoGrabber &cam){
    //        addFrame(cam.getPixels());
    //    }
    //
    //    void addFrame(ofVideoPlayer &player){
    //        addFrame(player.getPixels());
    //    }
};
