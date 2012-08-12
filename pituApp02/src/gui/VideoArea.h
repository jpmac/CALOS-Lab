//
//  VideoArea.h
//  pituApp02
//
//  Created by Patricio González Vivo on 8/10/12.
//  Copyright (c) 2012 PatricioGonzalezVivo.com. All rights reserved.
//

#ifndef VIDEOAREA
#define VIDEOAREA

#include "ofMain.h"

#include "ThresholdBar.h"

#include "ofxOpenCv.h"
#include "ofxXmlSettings.h"

class VideoArea : public ofRectangle{
public:
    VideoArea();
    
    void    allocate(int _width, int _height);
    
    void    save();
    
    void    update();
    void    updateGUI();
    
    void    draw();
    
    ThresholdBar        bar;
    
private:

    ofVideoGrabber      videoStream;
    
    ofxCvColorImage     cvColorImage;
    ofxCvGrayscaleImage cvGrayImage;
    ofxCvGrayscaleImage cvGrayImagePrevius;
    ofxCvGrayscaleImage cvDiffImage;
    ofxCvGrayscaleImage cvWarpImage;
    
    ofPoint             srcPoints[4];
    ofPoint             dstPoints[4];
    
    int                 pointSelected;
    int                 camWidth, camHeight;
};

#endif
