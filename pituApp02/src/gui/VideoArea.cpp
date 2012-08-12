//
//  VideoArea.cpp
//  pituApp02
//
//  Created by Patricio González Vivo on 8/10/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#include "VideoArea.h"

VideoArea::VideoArea(){
    camWidth = 640;
    camHeight = 480;

    bar.setLabel("video");
    
    pointSelected = -1;
    
}

void VideoArea::allocate(int _width, int _height){
    camWidth = _width;
    camHeight = _height;
    
    videoStream.initGrabber(camWidth,camHeight);
    
    cvColorImage.allocate(camWidth,camHeight);
    cvGrayImage.allocate(camWidth,camHeight);
    
    cvGrayImagePrevius.allocate(camWidth,camHeight);
    cvDiffImage.allocate(camWidth,camHeight);
    cvWarpImage.allocate(camWidth,camHeight);
    
    srcPoints[0] = dstPoints[0] = ofPoint(0,0);
    srcPoints[1] = dstPoints[1] = ofPoint(camWidth,0);
    srcPoints[2] = dstPoints[2] = ofPoint(camWidth,camHeight);
    srcPoints[3] = dstPoints[3] = ofPoint(0,camHeight);
    
    ofxXmlSettings XML;
    if (XML.loadFile("gui.xml")){
        XML.pushTag("videoArea",0);
        
        x = XML.getValue("x", 0.0);
        y = XML.getValue("y", 0.0);
        width = XML.getValue("width", 100.0);
        height = XML.getValue("height", 10.0);
        
        XML.pushTag("points",0);
        for(int i = 0; i < 4; i++){
            XML.pushTag("pts",i);
                srcPoints[i].x = XML.getValue("x", 0.0);
                srcPoints[i].y = XML.getValue("y", 0.0);
            XML.popTag();
        }
        XML.popTag();
        XML.popTag();
        XML.saveFile("gui.xml");
    }
}

void VideoArea::save(){
    if ( (pointSelected >= 0) && (pointSelected < 4)){
        
        ofxXmlSettings XML;
        if (XML.loadFile("gui.xml")){
            XML.pushTag("videoArea",0);
            
            XML.pushTag("points",0);

            XML.pushTag("pts",pointSelected);
            XML.setValue("x", srcPoints[pointSelected].x);
            XML.setValue("y", srcPoints[pointSelected].y);
            XML.popTag();
            
            XML.popTag();
            XML.popTag();
            XML.saveFile("gui.xml");
        }
    }
}

void VideoArea::update(){
    videoStream.update();
    
    if ( videoStream.isFrameNew()){
        cvColorImage.setFromPixels(videoStream.getPixels(), camWidth,camHeight);
        
        cvGrayImage = cvColorImage;
        cvDiffImage.absDiff(cvGrayImage, cvGrayImagePrevius);
        
        cvWarpImage.warpIntoMe(cvDiffImage, srcPoints, dstPoints);
        
        unsigned char* pixels = cvWarpImage.getPixels();
        
        int size = camWidth * camHeight;
        int tmp = 0;
        for (int i = 0; i < size; i++){
            tmp += pixels[i];
        }
        float average = (float)tmp/(float)size;
        average /= 255;
        
        bar.setValue(average);
        
        cvGrayImagePrevius = cvGrayImage;
    }
}

void VideoArea::updateGUI(){
    if ( ofGetMousePressed() ){
        
        if ( pointSelected == -1 ){
            for(int i = 0; i < 4; i++){
                if ( srcPoints[i].distance(ofPoint(ofGetMouseX(),ofGetMouseY())) < 20){
                    srcPoints[i].x = ofGetMouseX();
                    srcPoints[i].y = ofGetMouseY();
                    pointSelected = i;
                    break;
                }
            }
            
            if (pointSelected == -1){
                bar.updateGUI();
            } 
            
        } else {
            srcPoints[pointSelected].x = ofGetMouseX();
            srcPoints[pointSelected].y = ofGetMouseY();
            save();
        }
        
    } else {
        pointSelected = -1;
    }
    
    
}

void VideoArea::draw(){
    ofPushStyle();
    ofSetColor(255);
    cvColorImage.draw(0,0);
    for(int i = 0; i < 4; i++){
        ofSetColor(255,100);
        ofRect(srcPoints[i].x-5,srcPoints[i].y-5,10,10);
        ofLine(srcPoints[i], srcPoints[(i+1)%4]);
    }
    ofPopStyle();
    
    ofSetColor(255);
    bar.draw();
}