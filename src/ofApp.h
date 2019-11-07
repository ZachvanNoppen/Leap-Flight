#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "leapControls.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);

		void generateClouds();
		void deleteClouds();
		void tiltCamera(int keyPressed);
		void flapWings();

		const float TILT_TARGET = 5.0f;
		const float LERP_PERCENT = 0.5f;
		const float HEIGHT_MOD_TARGET = 4.0f;
		const int SPEED_INC = 4;

		ofLight m_light;

		ofCamera m_cam;
		ofVec3f m_cameraPosition;
		float m_cameraTilt;

		ofVec3f m_horizonPosition;
		ofVec3f m_birdPosition;
		ofNode m_birdNode;
		ofNode m_birdWingPosition[2];
		float m_wingAngle;
		float m_wingDirection = 1.0;

		vector<ofxAssimpModelLoader> m_clouds;
		float m_heightModifier;

		std::chrono::steady_clock::time_point m_timer;
		std::chrono::steady_clock::time_point m_flapTimer;

		//enumerators are given default values from 1 to n so RIGHT is 0, LEFT is 1
		enum Wings {RIGHT, LEFT};

		controls.setup();
};
