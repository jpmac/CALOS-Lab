#pragma once

#include "ofMain.h"

#include "VideoArea.h"
#include "ThresholdBar.h"

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
    
    void startSpin();
    void stopSpin(float &value);
	
    //  Video
    //
    VideoArea       video;
    
    //  Audio
    //
    void audioIn(float * input, int bufferSize, int nChannels); 
    ofSoundStream   soundStream;
    float           smoothedVol;
    ThresholdBar    micBar;
    
    //  Main threshold Bar
    //
    ThresholdBar    mainBar;
    
    //  Arrow
    //
    ofImage         arrow;
    ofImage         playImg;
    ofColor         arrowColor;
    float           arrowAngle, lerpVal;

    int             width, height;
    
    //  Flags
    //
    bool            bDebug, bPlay;
};
