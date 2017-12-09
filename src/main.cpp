#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
//    	ofSetupOpenGL(1080,1920,OF_WINDOW);			// <-------- setup the GL context
    
    
    //    https://forum.openframeworks.cc/t/dual-monitor-full-screen/13654/10
    
    // Get screen widths and heights from Quartz Services
    // See https://developer.apple.com/library/mac/documentation/GraphicsImaging/Reference/Quartz_Services_Ref/index.html
    
    CGDisplayCount displayCount;
    CGDirectDisplayID displays[32];
    
    // Grab the active displays
    CGGetActiveDisplayList(32, displays, &displayCount);
    int numDisplays= displayCount;
    
    // If two displays present, use the 2nd one. If one, use the first.
    int whichDisplay= numDisplays-1;
    
    int displayHeight= CGDisplayPixelsHigh ( displays[whichDisplay] );
    int displayWidth= CGDisplayPixelsWide ( displays[whichDisplay] );
    
    /* /////////////// this is a bit slow to do  /////////////
     // move 2nd display to right of primary display and align display tops
     if(numDisplays > 0){
     CGDisplayConfigRef displayConfig;
     CGBeginDisplayConfiguration ( &displayConfig );
     CGConfigureDisplayOrigin ( displayConfig, displays[1], CGDisplayPixelsWide(displays[0]), 0 );
     CGCompleteDisplayConfiguration ( displayConfig, kCGConfigureForAppOnly );
     }
     */
    //* //////// instead let's just moving our window to wherever our display is living:
    
    CGRect displayBounds= CGDisplayBounds ( displays[whichDisplay] );
    
    ofSetupOpenGL(displayWidth, displayHeight, OF_FULLSCREEN);            // <-------- setup the GL context
    // that OF_FULLSCREEN makes the window as big as the primary display, but we want it to be as big as whichever we're using
    ofSetWindowShape(displayWidth, displayHeight);
    // move onto our display.
    ofSetWindowPosition(displayBounds.origin.x, displayBounds.origin.y);
    
    // print display info.
    cout<<numDisplays<<" display(s) detected."<<endl<<"Using display "<<whichDisplay<<" ("<<displayWidth<<"x"<<displayHeight<<")."<<endl;
    
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());

}
