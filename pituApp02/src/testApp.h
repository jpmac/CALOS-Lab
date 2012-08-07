#pragma once

#include "ofMain.h"
#include "ofxInput.h"
#include "ofxOpenCv.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	
    //  Audio
    //
    void audioIn(float * input, int bufferSize, int nChannels); 
    ofSoundStream   soundStream;
    float           smoothedVol;
    ofxInput        micBar;
    
    //  Video
    //
    ofVideoGrabber      videoStream;
    ofxCvColorImage     cvColorImage;
    ofxCvGrayscaleImage cvGrayImage;
    ofxCvGrayscaleImage cvGrayImagePrevius;
    ofxCvGrayscaleImage cvDiffImage;
    ofxCvGrayscaleImage cvWarpImage;
    ofxInput            cvBar;
    ofPoint             srcPoints[4];
    ofPoint             dstPoints[4];
    
    ofImage     arrow;
    ofColor     arrowColor;
    float       arrowPos;
    float       lerpVal;

    int         width, height;
};
