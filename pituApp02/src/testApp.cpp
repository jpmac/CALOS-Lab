#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
	
    video.allocate(640, 480);
    
    //soundStream.listDevices();
	//if you want to set a different device id 
	//soundStream.setDeviceID(0); //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	soundStream.setup(this, 0, 2, 44100, 256, 4);
    // 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
    smoothedVol     = 0.0;
    micBar.setLabel("mic");
    
    mainBar.setLabel("main");
    ofAddListener(mainBar.thresholdTrigger, this, &testApp::stopSpin);
    
    //  Arrow
    //
    playImg.loadImage("play.png");
    arrow.loadImage("arrow.png");
    arrowAngle = 0.0;
    arrowColor.set(255,0,0);
    lerpVal = 0.5;
    
    bDebug = true;
    bPlay = false;
}

//--------------------------------------------------------------
void testApp::update(){
    
    video.update();
    
    micBar.setValue(smoothedVol);
    mainBar.setValue( (micBar.getActiveValue()+video.bar.getActiveValue())/(ofGetFrameRate()*0.1) );
    
    if (bDebug){
        video.updateGUI();
        micBar.updateGUI();
        mainBar.updateGUI();
    }
    
    if (bPlay){
        arrowAngle += lerpVal ;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    if (bDebug){
        //  Draw the average movement
        //
        ofSetColor(255);
        video.draw();
        
        //  Draw the average volume
        //
        ofSetColor(255);
        micBar.draw();
        
    } else {
    
        //  Dibuja la Flecha
        //
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()*0.5, 
                    ofGetHeight()*0.5);
        
        ofRotate(arrowAngle);
        ofTranslate(-arrow.getWidth()*0.25, 
                    -arrow.getHeight()*0.5);
        
        ofSetColor(arrowColor);
        arrow.draw(0, 0);
        
        ofPopMatrix();
        ofPopStyle();
        
        playImg.draw(ofGetWidth()*0.5-30,ofGetHeight()*0.5-30, 60, 60 );
    }
    mainBar.draw();
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	float curVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	
    
    float left[bufferSize];
    float right[bufferSize];
    
	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;
        
		curVol += left[i] * left[i];
		curVol += right[i] * right[i];
		numCounted+=2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
}

void testApp::startSpin(){
    bPlay = true;
}

void testApp::stopSpin(float &value){
    bPlay = false;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'd':
            bDebug = !bDebug;
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (!bDebug){
        if (ofDist(ofGetWidth()*0.5, ofGetHeight()*0.5, x, y) < 60){
            bPlay = !bPlay;
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}