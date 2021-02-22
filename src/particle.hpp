#pragma once
#include "ofMain.h"
#include <string>
#include <iostream>
#include <fstream>

class Particles{
public:
    Particles(string _filename, int _N);
    Particles();
    void update();
    void draw();
    bool can_file_use();
    void zoom_up();
    void zoom_down();
    void slide_right();
    void slide_left();
    void slide_up();
    void slide_down();
    void command_list();

    float radius;
    int N;
    ifstream inputData;
    float zoom_rate = 1.0;
    float slide_x = 0.0;
    float slide_y = 0.0;
    long int counter = 0;
    bool stop_flag = true;

    vector<ofVec2f> r;

};