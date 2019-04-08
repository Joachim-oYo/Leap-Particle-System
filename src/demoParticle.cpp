#include "demoParticle.h"

//------------------------------------------------------------------
demoParticle::demoParticle(){
    attractPoints = NULL;
}

//------------------------------------------------------------------
void demoParticle::setMode(particleMode newMode){
    mode = newMode;
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints( vector <ofPoint> * attract ){
    attractPoints = attract;
}

//------------------------------------------------------------------
void demoParticle::reset(){
    //the unique val allows us to set properties slightly differently for each particle
    uniqueVal = ofRandom(-10000, 10000);
    originalColor = ofColor::fromHsb(ofRandom(0,255), 200, 255);
//    originalColor = ofColor(ofRandom(50, 255), ofRandom(50, 255), ofRandom(50, 255));
    currentColor = originalColor;
    
    pos.x = ofRandomWidth();
    pos.y = ofRandomHeight();
    
    vel.x = ofRandom(-3.9, 3.9);
    vel.y = ofRandom(-3.9, 3.9);
    
    frc   = ofPoint(0,0,0);
    
    scale = ofRandom(0.5, 1.0);
    
    drag  = ofRandom(0.95, 0.998);
}

//------------------------------------------------------------------
void demoParticle::update(){
    
    //1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN
    
    if( mode == PARTICLE_MODE_ATTRACT ){
        if (count < 1)
            count += (0.0008) * speedCoefficient;
        ofPoint attractPt(cursor.x, cursor.y);
        frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
        float dist = frc.length();
        frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point
        
        vel *= drag; //apply drag
        
        if (dist > 300) {
            vel += frc * 0.0009 * speedCoefficient;
        }
        else if (dist > 50+ofRandom(-30, 20)) {
            vel += frc * 0.006 * speedCoefficient;
        }
        else {
            frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
            frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
            vel -= frc * 0.04 * speedCoefficient;
        }
        
        //vel += frc * 0.006; //apply force
    }
    else if( mode == PARTICLE_MODE_REPEL ){
        count = 0;
        
        ofPoint attractPt(cursor.x, cursor.y);
        frc = (attractPt-pos);
        
        //let get the distance and only repel points close to the mouse
        float dist = frc.length();
        frc.normalize();
        
        vel *= drag;
        
        int offset = ofRandom(20);
        
        inActiveArea = (dist < 230);
        
        if (inActiveArea) {
            if( dist < 150+ofRandom(20) ){
                vel += -frc * 0.6*speedCoefficient; //notice the frc is negative
            }
        }
        else{
            //if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
            frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
            frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
            vel += frc * 0.004*speedCoefficient;
        }
    }
    else if ( mode == PARTICLE_MODE_OFF){
        inActiveArea = false;
    }
    
    //2 - UPDATE OUR POSITION
    
    pos += vel;
    
    
    //3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN
    //we could also pass in bounds to check - or alternatively do this at the ofApp level
    if( pos.x > 1920 ){
        pos.x = 1920;
        vel.x *= -1.0;
    }else if( pos.x < 0 ){
        pos.x = 0;
        vel.x *= -1.0;
    }
    if( pos.y > 1080 ){
        pos.y = 1080;
        vel.y *= -1.0;
    }
    else if( pos.y < 0 ){
        pos.y = 0;
        vel.y *= -1.0;
    }
    
}

//------------------------------------------------------------------
void demoParticle::draw(){
    if( mode == PARTICLE_MODE_ATTRACT ){
        ofColor transitionColor = originalColor;
        currentColor = transitionColor.lerp(energizedColor, count);
    }
    else if( mode == PARTICLE_MODE_REPEL ){
        currentColor = originalColor;
    }
    else if ( mode == PARTICLE_MODE_OFF ){
        currentColor = originalColor;
    }
    
    if (inActiveArea)
        ofSetColor(currentColor.r, currentColor.g + 120, currentColor.b);
    else
        ofSetColor(currentColor);
    
    ofDrawCircle(pos.x, pos.y, scale * 4.0);
}
