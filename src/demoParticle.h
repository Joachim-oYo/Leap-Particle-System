#pragma once
#include "ofMain.h"

enum particleMode{
    PARTICLE_MODE_ATTRACT = 0,
    PARTICLE_MODE_REPEL,
    PARTICLE_MODE_OFF
};

class demoParticle{
    
public:
    demoParticle();
    
    void setMode(particleMode newMode);
    void setAttractPoints( vector <ofPoint> * attract );
    
    void reset();
    void update();
    void draw();
    
    ofPoint pos;
    ofPoint vel;
    ofPoint frc;
    ofPoint cursor = ofPoint(0,0);
    
    float drag;
    float uniqueVal;
    float scale;
    
    particleMode mode;
    
    vector <ofPoint> * attractPoints;
    ofColor originalColor;
    ofColor currentColor;
    ofColor energizedColor = ofColor(80, 200, 120);
    bool inActiveArea;
    
    float count;
    int speedCoefficient = 19;
};
