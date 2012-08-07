//
//  File.cpp
//  pituApp02
//
//  Created by Patricio González Vivo on 8/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#include "ofxInput.h"

ofxInput::ofxInput(){
    x = 0;
    y = 0;
    width = 100;
    height = 10;
    
    minValue = 0.0;
    maxValue = 1.0;
    
    color.set(255,0,0);
    
    vertOriented = false;
};

void ofxInput::setValue(float _value){
    value = ofMap(_value, minValue, maxValue, 0.0, 1.0, true);
};

void ofxInput::update(){
    if ( ofGetMousePressed() ){
        float mouseX = ofGetMouseX();
        float mouseY = ofGetMouseY();
        
        if ((mouseX >= x ) && 
            (mouseX <= x + width) &&
            (mouseY >= y ) && 
            (mouseY <= y + height)){
            
            if ( !vertOriented ){
                threshold = (mouseX - x) / width;
            } else {
                threshold = (mouseY - y) / height;
            }
        }
    }
    
    color.setHue(value*100);
};

void ofxInput::draw(){
    // draw the average volume:
	ofPushStyle();
    ofPushMatrix();
    ofTranslate(x, y, 0);
    
    
    if ( !vertOriented ){
        
        //  Box bar
        //
        ofNoFill();		
        ofSetColor(255,100);
        ofRect(0, 0, width, height);
        
        //  Value bar
        //
        float padding = height*0.2;
        ofFill();
        ofSetColor(color,200);
        ofRect( padding, padding, min(value*width, width-padding*2) , height-padding*2);
    
        //  Threshold Line
        //
        ofSetColor(255,150);
        ofLine(threshold*width, 0, 
               threshold*width, height);
    } else {
        //  Box bar
        //
        ofNoFill();		
        ofSetColor(255,100);
        ofRect(0, 0, width, height);
        
        //  Value bar
        //
        float padding = width*0.2;
        ofFill();
        ofSetColor(color,200);
        ofRect( padding,height - padding - min( value*height + padding, height + padding) , width-padding*2 , min(value*height,height-padding*2) );
        
        //  Threshold Line
        //
        ofSetColor(255,150);
        ofLine(0, threshold*height, 
               width, threshold*height);
    }
    
    ofPopMatrix();
	ofPopStyle();
};