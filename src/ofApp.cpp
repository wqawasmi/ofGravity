#include "ofApp.h"
#include <iostream>
#include <cmath>

int spawnRadius = 400;
int numParticles = 8000;
float maxVel = 0;


class Particle {
private:
    ofPoint accel;
    
public:
    bool repel;
    float mass;
    float minDistance;
    ofPoint pos;
    ofPoint vel;
    ofPoint force;
    
    void refresh();
    void render();
    void applyForce(ofPoint _force);
    ofPoint spawn();
    
    //Constructor
    Particle();
    
    //Gravity function
    ofPoint gravitate(Particle p);
    
    //Get magnitude of velocity
    float getMagnitude();
};

Particle::Particle() {
    repel = false;
    mass = 1;
    minDistance = 50;
    pos = spawn();
    vel = ofPoint(0,0);
    force = ofPoint(0,0);
    accel = ofPoint(0,0);
}

//Random spawn within the circle defined by spawnRadius in systemSettings.h
ofPoint Particle::spawn() {
    float x = ofRandom(-1, 1);
    float y = ofRandom(-1, 1);
    
    x *= spawnRadius;
    y *= spawnRadius;
    
    x += ofGetWindowWidth()/2;
    y += ofGetWindowHeight()/2;
    return ofPoint(x, y);
}

void Particle::applyForce(ofPoint _force) {
    force = _force;
}

//Update vars based on Acceleration = Force/Mass
void Particle::refresh() {
    accel.x = force.x/mass;
    accel.y = force.y/mass;

    vel+= accel;
    pos += vel;
    
    //Restrict pos to window bounds
    //Find glancing direction by adding 180 - 2theta degrees
    
    //Left/Right border collision
    if(pos.x <= 0 || pos.x >= ofGetWindowWidth()) {
        float theta = vel.angle(ofPoint(0, 1));
        vel.rotate(180, ofPoint(0,1));
    }
    
    //Top/Bottom border collision
    if(pos.y <= 0 || pos.y >= ofGetWindowHeight()) {
        float theta = vel.angle(ofPoint(1, 0));
        vel.rotate(180, ofPoint(1,0));
    }
        
    accel = ofPoint(0,0);
}

void Particle::render() {
    ofCircle(pos.x, pos.y, 0.8);
}

//Using F = GMm/r2 with high G to account for low masses
ofPoint Particle::gravitate(Particle p) {
    
    float distance = p.pos.distance(pos);
    float G = 70; //Really big
   
    
    if(distance <= minDistance)
        distance = minDistance;
    
    float mag = G * mass * p.mass / (distance * distance);
    ofPoint dir = p.pos - pos;
    
    if(p.repel)
        dir.scale(-1);
    
    ofPoint gForce = dir.scale(mag);
    
    return gForce;
}

float Particle::getMagnitude() {
    return sqrt(pow(vel.x, 2) + pow(vel.y, 2));
}

//--------------------------------------------------------------
//ofApp starts here
//--------------------------------------------------------------

vector<Particle> world;
vector<Particle> attraction; //Points of gravitational attraction
vector<Particle> replusion;

//--------------------------------------------------------------
void ofApp::setup(){
    for(int i = 0; i < numParticles; i++) {
        world.push_back(Particle());
    }
    
    Particle center = Particle();
    center.pos = ofPoint(1024/2, 768/2);
    attraction.push_back(center);

    ofBackground(45);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPoint force;
    
    for(int i = 0; i < numParticles; i++) {
        //Find sum of gravitational forces on current particle
        for(int j = 0; j < attraction.size(); j++) {
            if(j == 0)
                force = (world[i].gravitate(attraction[j]));
            else
                force += (world[i].gravitate(attraction[j]));
        }
        
        world[i].applyForce(force);
        world[i].refresh();
        
        //Calculate color based on velocity
        maxVel = 2.4;
        int colorCode =  (int)ofMap(world[i].getMagnitude(), 0, maxVel, 100, 255);
        ofSetColor(colorCode, 255 - colorCode, 0);
        world[i].render();
    }
    
    for(int i = 0; i < attraction.size(); i++) {
        ofSetColor(0);
        ofCircle(attraction[i].pos.x, attraction[i].pos.y, 2);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    Particle p = Particle();
    p.pos = ofPoint(x, y);

    if(button == 1) {
        attraction.clear();
        return;
    }
    
    if(button == 2)
        p.repel = true;
    
    attraction.push_back(p);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
