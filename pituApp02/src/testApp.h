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
    
    //  Areas
    //
    ofPolyline      *areas;
    float           areaAngle;
    int             areaNum;
    int             areaTarget;
    int             areaSelected;
    
    //  Arrow
    //
    ofImage         arrow;
    ofImage         playImg;
    ofPoint         center;
    ofPoint         pointingTo;
    ofColor         arrowColor;
    float           arrowAngle;
    float           arrowLerp;

    int             width, height;
    
    //  Flags
    //
    bool            bDebug, bPlay;
    
    
};
