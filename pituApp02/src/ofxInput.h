//
//  ofxInputSensorBar.h
//  pituApp02
//
//  Created by Patricio González Vivo on 8/6/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef OFXINPUTSENSORBAR
#define OFXINPUTSENSORBAR

#include "ofMain.h"

class ofxInput : public ofRectangle {
public:
    
    ofxInput();

    void    setMin(float _minValue){minValue = _minValue;};
    void    setMax(float _maxValue){maxValue = _maxValue;};
    
    void    setVerticalOriented( bool _vert = true){ vertOriented = _vert;};
    void    setThreshold(float _threshold){threshold = _threshold;};
    float   getThreshold(){return threshold;};
    
    void    setValue(float _value);
    float   getValue(){return value;};
    float   getActiveValue(){
        if ( value >= threshold ){
            return value - threshold;
        } else {
            return 0.0;
        }
    };
    
    void    update();
    void    draw();
    
private:
    
    ofColor color;
    float   maxValue;
    float   minValue;
    float   threshold;
    float   value;
    
    bool    vertOriented;
};
#endif
