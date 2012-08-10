#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	 
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
	
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
    
    micBar.setMax(0.1);
    micBar.setThreshold(0.1);
    micBar.x = 30;
    micBar.y = height + 30;
    
    videoStream.initGrabber(width,height);
    cvColorImage.allocate(width, height);
    cvGrayImage.allocate(width, height);
    cvGrayImagePrevius.allocate(width, height);
    cvDiffImage.allocate(width, height);
    cvWarpImage.allocate(width, height);
    srcPoints[0] = dstPoints[0] = ofPoint(0,0);
    srcPoints[1] = dstPoints[1] = ofPoint(width,0);
    srcPoints[2] = dstPoints[2] = ofPoint(width,height);
    srcPoints[3] = dstPoints[3] = ofPoint(0,height);
    cvBar.setMax(0.1);
    cvBar.setThreshold(0.1);
    cvBar.x = srcPoints[3].x + 30;
    cvBar.y = srcPoints[3].y + 20;

    mainBar.setMax(0.3);
    mainBar.setThreshold(0.1);
    mainBar.x = 10;
    mainBar.y = 10;
    mainBar.width = 10;
    mainBar.height = 100;
    mainBar.setVerticalOriented();
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
    
    micBar.setValue(smoothedVol);
    micBar.update();
    
    videoStream.update();
    
    if ( videoStream.isFrameNew()){
        cvColorImage.setFromPixels(videoStream.getPixels(), width, height);
        
        cvGrayImage = cvColorImage;
        cvDiffImage.absDiff(cvGrayImage, cvGrayImagePrevius);
        
        cvWarpImage.warpIntoMe(cvDiffImage, srcPoints, dstPoints);
        
        unsigned char* pixels = cvWarpImage.getPixels();
        
        int size = width * height;
        int tmp = 0;
        for (int i = 0; i < size; i++){
            tmp += pixels[i];
        }
        float average = (float)tmp/(float)size;
        average /= 255;
        
        cvBar.setValue(average);
        cvBar.update();
        
        cvGrayImagePrevius = cvGrayImage;
    }
    
    mainBar.setValue((micBar.getActiveValue()+cvBar.getActiveValue())/(ofGetFrameRate()*0.1));
    mainBar.update();
    
    if (bPlay){
        arrowAngle += lerpVal ;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    if (bDebug){
        ofPushStyle();
        ofSetColor(255);
        cvColorImage.draw(0,0);
        for(int i = 0; i < 4; i++){
            ofSetColor(255,100);
            ofRect(srcPoints[i].x-5,srcPoints[i].y-5,10,10);
            ofLine(srcPoints[i], srcPoints[(i+1)%4]);
        }
        ofPopStyle();
        
        //  Draw the average volume:
        //
        ofSetColor(255);
        ofDrawBitmapString("m:", 5, height + 39 );
        micBar.draw();
        ofSetColor(255);
        ofDrawBitmapString("v:", srcPoints[3].x , srcPoints[3].y + 29 );
        cvBar.draw();
    } else {
    
        //  Dibuja la Flecha
        //
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(ofGetWidth()*0.5, 
                    ofGetHeight()*0.5);
        
        ofRotate(arrowAngle);
        ofTranslate(-arrow.getWidth()*0.1, 
                    -arrow.getHeight()*0.5);
        
        ofSetColor(arrowColor);
        arrow.draw(0, 0); //,arrow.getWidth()*0.5,arrow.getHeight()*0.5);
        
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
    int selected = -1;
    for(int i = 0; i < 4; i++){
        if ( srcPoints[i].distance(ofPoint(x,y)) < 50){
            srcPoints[i].x = x;
            srcPoints[i].y = y;
            selected = i;
            break;
        }
    }
    
    if (selected == 3){
        cvBar.x = srcPoints[3].x + 20;
        cvBar.y = srcPoints[3].y + 20;
    }
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