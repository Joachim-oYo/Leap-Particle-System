#pragma once

#include "ofMain.h"
#include "ofxLeapMotion2.h"
#include "ofxParagraph.h"
#include "demoParticle.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void resetParticles();
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
	ofxLeapMotion leap;
	vector <ofxLeapMotionSimpleHand> simpleHands;
    ofVec2f fingerTips[5];
    ofVec2f centroid;
    float distFromPalm[5];
    
    ofxParagraph paragraph;
    
	vector <int> fingersFound;
	ofEasyCam cam;
    
    
    particleMode currentMode;
    string currentModeStr;
    
    vector <demoParticle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;
    
    float fistThreshold = 240;
    float bHide = true;
    string fistStrengthStr;
    float lastRecordedTime;
    
};
