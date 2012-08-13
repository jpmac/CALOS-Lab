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
    
    //  Areas
    areaNum = 3;
    areas = new ofPolyline[areaNum];
    areaAngle = (360.0/(float)(areaNum));
    center = ofPoint(ofGetWidth()*0.5,ofGetHeight()*0.5);
    for(int i = 0; i < areaNum; i++){
        
        int startAngle = i*areaAngle + areaAngle + 90*3;
        int endAngle = ((i+1)%areaNum)*areaAngle + areaAngle + 90*3;
        
        areas[i].addVertex(center);
        areas[i].arc(center, ofGetHeight()*0.5, ofGetHeight()*0.5, startAngle, endAngle, true, 360);
        areas[i].close();
    }
    
    //  Arrow
    //
    playImg.loadImage("play.png");
    arrow.loadImage("arrow.png");
    arrowAngle = 0.0;
    arrowColor.set(255,0,0);
    arrowColor.setHue(100);
    arrowLerp = 0.5;
    
    areaTarget = -1;
    
    bDebug = false;
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
        arrowAngle += arrowLerp;
    } else if (areaTarget != -1){
        
        if ( pointingTo.distance( areas[areaTarget].getCentroid2D() ) < 50 ){
            areaTarget = -1;
        } else {
            ofPoint targetPosition;
            targetPosition = areas[areaTarget].getCentroid2D();
            targetPosition = targetPosition - center;
            float targetAngle = ofRadToDeg(atan2(targetPosition.y,targetPosition.x));
            arrowAngle = ofLerp(arrowAngle, targetAngle, 0.1);
        }
    }
    
    if (arrowAngle > 360){
        arrowAngle -= 360;
    }
    
    pointingTo.x = cos( ofDegToRad(arrowAngle) ) * (ofGetWidth()*0.25);
    pointingTo.y = sin( ofDegToRad(arrowAngle) ) * (ofGetHeight()*0.25);
    pointingTo = pointingTo + center;
    
    for(int i = 0; i < areaNum; i++){
        if( areas[i].inside( pointingTo ) ){
            areaSelected = i;
            break;
        }
    }
    
    cout << "Angle " << arrowAngle << endl;
    cout << "Selected " << areaSelected << endl;
    cout << "Target " << areaTarget << endl;
    cout << endl;
    
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
    
        //  Draw areas
        //
        ofPushStyle();
        ofColor color;
        color.set(255,0,0);
        ofFill();
        for(int i = 0; i < areaNum; i++){
            float hue = float(i)/float(areaNum);
            if (i == 0){
                ofSetColor(100);
            } else if ( i == 1){
                ofSetColor(0, 255, 0);
            } else if ( i == 2){
                ofSetColor(255, 0, 0);
            }
            ofBeginShape();
            ofVertexes(areas[i].getVertices());
            ofEndShape();
        }
        ofPopStyle();
        
        //  Draw the arrow
        //
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()*0.5, 
                    ofGetHeight()*0.5);
        
        ofRotate(arrowAngle);
        ofTranslate(-arrow.getWidth()*0.25, 
                    -arrow.getHeight()*0.5);
        
        ofSetColor(255);
        arrow.draw(0, 0);
        
        ofPopMatrix();
        ofPopStyle();
        
        ofSetColor(255);
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
    if ( bPlay ){
        bPlay = false;
    } else if ( areaTarget == -1 ){
        areaTarget = (areaSelected+1)%areaNum;
    }
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
    center = ofPoint(ofGetWidth()*0.5,ofGetHeight()*0.5);
    
    for(int i = 0; i < areaNum; i++){
        
        int startAngle = i*areaAngle + areaAngle + 90*3;
        int endAngle = ((i+1)%areaNum)*areaAngle + areaAngle + 90*3;
        
        areas[i].clear();
        areas[i].addVertex(center);
        areas[i].arc(center, ofGetHeight()*0.5, ofGetHeight()*0.5, startAngle, endAngle, true, 360);
        areas[i].close();
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}