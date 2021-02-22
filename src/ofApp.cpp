#include "ofApp.h"
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;
//--------------------------------------------------------------
std::tuple<int,float,string,string> simulation_setting(){
    string path = "/Applications/of_v0.11.0_osx_release/apps/myApps/simulation_plot_movie/data/";
    string filename = "sample.dat";

    int N = 10000;
    float rho = 0.5;
    float L = sqrt(N/rho);
    return std::forward_as_tuple(N,L, path + filename,filename);
}

void ofApp::setup(){
    ofSetCircleResolution(12);
    ofSetFrameRate(10);
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    font.load("arial.ttf", 25);

    std::tie(N,system_size,path,filename) = simulation_setting();

    p = Particles(path,N);
    if(system_size > window_size){
        cout << "system_size is larger than windowsize\n";
    }
    p.zoom_rate = static_cast<float>(window_size) / static_cast<float>(system_size);
    ofSetWindowShape(window_size, window_size);
    //ofSetWindowPosition(0,0);
    //ofSetBackgroundColor(255,255,255);
    //ofSetColor(65,105,225,200);

    //For movie
    //m_Grabber.setup(ofGetWidth(), ofGetHeight());
    mCapFbo.allocate( ofGetWidth(),ofGetHeight(), GL_RGB );

    m_Recorder.setup(true, false, glm::vec2(ofGetWidth(), ofGetHeight()) );
    m_Recorder.setAudioConfig(1024,44100);
    m_Recorder.setOverWrite(true);

    #if defined(TARGET_OSX)
    m_Recorder.setFFmpegPath(ofToDataPath("ffmpeg/osx/ffmpeg"));
    #elif defined(TARGET_WIN32)
    m_Recorder.setFFmpegPath(ofToDataPath("ffmpeg/win/ffmpeg.exe"));
    #endif

    isRecordingVideo = false;
    isRecordingAudio = false;

    audioFPS = 0.0f;
    audioCounter    = 0;
    bufferCounter	= 0;
    lastAudioTimeReset = ofGetElapsedTimeMillis();

    //Finish setup
    cout << "Press h for help menu\n";
}

//--------------------------------------------------------------
void ofApp::update(){
    if(p.stop_flag && p.can_file_use()){
        p.update();
    }
    //m_Grabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

    if(!p.can_file_use()){
        ofSetColor(0,0,0);
        font.drawString("This movie got ot the end", 50, 50);
    }

    //capture snapshots image
    mCapFbo.begin(); {
        //We need to clear the buffer because capture board does not clear buffer automatically.
        ofClear(255,255,255);
        ofSetBackgroundColor(255,255,255);
        ofSetColor(65,105,225,200);
        //m_Grabber.draw(0, 0, mCapFbo.getWidth(), mCapFbo.getHeight() );
        p.draw();
    } mCapFbo.end();


    if( m_Recorder.isRecording() ) {
        if(isRecordingVideo){
            // ofxFastFboReader can be used to speed this up :)
            mCapFbo.readToPixels( mPix );
            if( mPix.getWidth() > 0 && mPix.getHeight() > 0 ) {
                m_Recorder.addFrame( mPix );
            }
        }
    }

    //show snapshots from capture board
    mCapFbo.draw(0,0);

    //Print record timer
    if(isRecordingVideo){
        ofDrawBitmapStringHighlight(std::to_string(m_Recorder.getRecordedDuration()), 40, 45);
    }else if(isRecordingAudio){
        ofDrawBitmapStringHighlight(std::to_string(m_Recorder.getRecordedAudioDuration(audioFPS)), 40, 45);
    }else{
        ofDrawBitmapStringHighlight(std::to_string(m_Recorder.getRecordedDuration()), 40, 45);
    }

    ofPushStyle(); {
        if (m_Recorder.isPaused() && m_Recorder.isRecording()) {
            ofSetColor(ofColor::yellow);
        }
        else if (m_Recorder.isRecording()) {
            ofSetColor(ofColor::red);
        }
        else {
            ofSetColor(ofColor::green);
        }
        ofDrawCircle(ofPoint(10, 40), 10);
        ofSetColor(65,105,225,200);
    }
    ofPopStyle();

}

//--------------------------------------------------------------
string split(const string &s, char delim){
    vector<string> elems;
    stringstream ss(s);
    string item;
    getline(ss, item, delim);
    return item;
}

string zero_padding_filename(int i){
    std::string str;
    std::ostringstream stream;

    stream.str("");
    stream << std::setfill('0') << std::setw(5) << i;
    str = stream.str();
    std::cout << std::setfill(' ');
    return str;
}

void ofApp::keyPressed(int key){
    switch(key) {
        case 'q':
            cout << "Bye\n";
            std::exit(0);
        case 'u':
            p.zoom_up();
        break;
        case 'd':
            p.zoom_down();
        break;
        case OF_KEY_UP:
            p.slide_up();
        break;
        case OF_KEY_DOWN:
            p.slide_down();

        break;
        case OF_KEY_LEFT:
            p.slide_left();

        break;
        case OF_KEY_RIGHT:
            p.slide_right();
        break;
        case 'h':
            p.command_list();
        break;
        case 't':
            p.stop_flag = !p.stop_flag;
        break;
        case 's':
            mCapFbo.draw(0,0);
            snapshot.grabScreen(0,0,ofGetWidth(),ofGetHeight());
            string dir_name = create_snapshot_directory_name();
            cout << "save at " << dir_name << "\n";
            if(!fs::exists(dir_name)){
                fs::create_directory(dir_name);
            }
            string snapshot_name = zero_padding_filename(p.counter);

            snapshot.save(dir_name + "/" + snapshot_name + ".png");
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' ') {

        if( m_Recorder.isRecording() ) {
            // stop
            m_Recorder.stop();
            isRecordingVideo = false;
        } else {
#if defined(TARGET_OSX)
            m_Recorder.setOutputPath( ofToDataPath(ofGetTimestampString() + ".mp4", true ));
#else
            m_Recorder.setOutputPath( ofToDataPath(ofGetTimestampString() + ".avi", true ));
#endif

            m_Recorder.setVideoCodec("libx264");
            m_Recorder.setBitRate(8000);

            isRecordingVideo = true;
            m_Recorder.startCustomRecord();
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
string ofApp::create_snapshot_directory_name(){
    string snapshot_path = "/Applications/of_v0.11.0_osx_release/apps/myApps/simulation_plot_movie/snapshots/";
    string snapshot_filename = split(filename, '.');
    return snapshot_path + snapshot_filename;
}
