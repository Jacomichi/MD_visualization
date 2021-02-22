#include "particle.hpp"
#include "ofApp.h"


void initialize_position(vector<ofVec2f> r,int N){
    for(int i =0;i < N;++i){
        r[i].x = 0.0;
        r[i].y = 0.0;
    }
}

Particles::Particles(string _filename, int _N){
    inputData.open(_filename);
    if (inputData.fail())
    {
        cout << "ファイルが開けませんでした。" << endl;
        exit(1);
    }

    N = _N;
    radius = 1.0;
    vector<ofVec2f> _r(_N);
    r = _r;
    initialize_position(r,_N);
}

Particles::Particles(){

}


void Particles::update(){
    for(int i =0;i < N;++i){
        if(inputData.bad() || inputData.eof()){
            cout << "end of file\n";
            break;
        }
        inputData >> r[i].x >> r[i].y;
    }
    counter += 1;
}

void Particles::draw(){
    for(int i =0;i < N;++i){
        ofDrawCircle((r[i].x + slide_x) * zoom_rate, (r[i].y+ slide_y)  * zoom_rate,radius/2.0* zoom_rate);
    }
}

bool Particles::can_file_use(){
    bool flag = (inputData.bad() || inputData.eof());
    return !flag;
}

void Particles::zoom_up(){
    zoom_rate *= 1.05;
}

void Particles::zoom_down(){
    zoom_rate /= 1.05;
}

void Particles::slide_right(){
    slide_x -= ofGetWidth()/400;
}

void Particles::slide_left(){
    slide_x += ofGetWidth()/400;
}

void Particles::slide_up(){
    slide_y += ofGetHeight()/400;
}

void Particles::slide_down(){
    slide_y -= ofGetHeight()/400;
}

void Particles::command_list(){
    cout << "command list\n";
    cout << "q : quit\n";
    cout << "u : zoom up\n";
    cout << "d : zoom down\n";
    cout << "UP : slide up\n";
    cout << "DOWN : slide down\n";
    cout << "LEFT : slide left\n";
    cout << "RIGHT : slide right\n";
    cout << "s : save configuration as snapshot\n";
    cout << "space key : record the movie\n";
}
