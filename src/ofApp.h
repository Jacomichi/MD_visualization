#pragma once

#include "ofMain.h"
#include "particle.hpp"
#include "ofxFFmpegRecorder.h"
#include "ofVideoGrabber.h"

class ofApp : public ofBaseApp{
	private:
		ofTrueTypeFont font;

		//For movie
			ofxFFmpegRecorder m_Recorder;
		ofVideoGrabber m_Grabber;

		ofSoundStream soundStream;
		short* shortBuffer;
		ofPolyline          waveform;
		size_t  lastAudioTimeReset;
		int bufferCounter;
		float audioFPS;
		int audioCounter;
		
		ofFbo mCapFbo;
		ofPixels mPix;

    bool     isRecordingVideo;
    bool     isRecordingAudio;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		string create_snapshot_directory_name();
		
		Particles p;
		string path;
		string filename;
		int N;
		float system_size;

		float window_size = 800;

		ofImage snapshot;

};
