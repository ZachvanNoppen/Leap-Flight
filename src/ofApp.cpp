#include "ofApp.h"
#include "ofxAssimpModelLoader.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetSmoothLighting(true);
	ofSetVerticalSync(true);
	ofEnableLighting();

	m_light.setDirectional();
	m_light.setPosition(-1000, 1000, -50);
	m_light.lookAt(m_birdPosition);
	//Hard set Horizon position
	m_horizonPosition.set(0,-2400,-6000);
	m_cam.setPosition(0, 0, 0);
	m_birdPosition.set(0,-10,-30);


	m_cameraTilt = 0;
	m_timer = std::chrono::steady_clock::now()
		+ std::chrono::seconds(5);
	//m_flapTimer = std::chrono::steady_clock::now()
	  //+ std::chrono::seconds(1);

	m_heightModifier = 0;
	//Parenting camera to bird so all transformations are kept
	m_birdNode.setPosition(m_birdPosition);
	m_birdNode.setParent(m_cam);

	controls.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
	//dividing the camera tilt to stop the cam from rotating.
	m_cameraTilt /= 1.2f;
	m_cam.tiltDeg(m_cameraTilt);
	//m_wingAngle /= 1.009;

	//Creating new clouds and removing ones that are out of the frame
	generateClouds();
	deleteClouds();

	//Moving all of the coulds
	for (int inc = 0; inc < m_clouds.size(); inc++) {
		ofVec3f currentPos = m_clouds[inc].getPosition();
		//Note that they are always moving toward the camera
		//Note that they are moving up and down depending on the camera angle
		m_clouds[inc].setPosition(currentPos.x, currentPos.y + m_heightModifier, currentPos.z + SPEED_INC);
	}

	controls.update();

	m_birdPosition.x = controls.m_birdPos.x;
	m_birdPosition.y = controls.m_birdPos.y;

	m_birdNode.setPosition(m_birdPosition);

	//flapWings();
	m_birdWingPosition[RIGHT].setPosition(m_birdPosition.x + 3.0f, m_birdPosition.y, m_birdPosition.z);
	m_birdWingPosition[LEFT].setPosition(m_birdPosition.x - 3.0f, m_birdPosition.y, m_birdPosition.z);
	ofQuaternion orientation = m_birdNode.getGlobalOrientation();

	m_birdWingPosition[RIGHT].setGlobalOrientation(orientation);
	m_birdWingPosition[LEFT].setGlobalOrientation(orientation);

	//flapWings();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableLighting();
	ofEnableDepthTest();
	m_cam.begin();
	m_light.enable();

	//Drawing the bird
	//ofBoxPrimative as a placeholder for bird
	ofBoxPrimitive birdBody(1, 1, 10);
	//Creating boxes with a defined size
	ofBoxPrimitive birdWings[2];
	birdWings[RIGHT].set(5.0f, 0.5f, 4.0f);
	birdWings[LEFT].set(5.0f, 0.5f, 4.0f);


	ofSetColor(0, 0, 255);
	//Setting the position and orentation

	birdBody.setPosition(m_birdPosition);
	birdBody.setGlobalOrientation(m_birdNode.getGlobalOrientation());

	//Setting position and orientation
	birdWings[RIGHT].setPosition(m_birdWingPosition[0].getGlobalPosition());
	birdWings[RIGHT].setGlobalOrientation(m_birdWingPosition[0].getGlobalOrientation());
	birdWings[RIGHT].rollDeg(controls.m_wingAngle2);
	birdWings[LEFT].setPosition(m_birdWingPosition[1].getGlobalPosition());
	birdWings[LEFT].setGlobalOrientation(m_birdWingPosition[1].getGlobalOrientation());
	birdWings[LEFT].rollDeg(-controls.m_wingAngle1);
	//Rolling is used to flap the wings
	ofVec3f axis;
	axis.set(0.0f, 0.0f, 1.0f);
	birdWings[RIGHT].rotateAroundDeg(controls.m_wingAngle2, axis, m_birdPosition);
	birdWings[LEFT].rotateAroundDeg(-controls.m_wingAngle1, axis, m_birdPosition);

	birdWings[RIGHT].draw();
	birdWings[LEFT].draw();
	birdBody.draw();


	//Drawing the horizon line
	ofSetColor(0, 0, 0);
	ofFill();
	ofDrawBox(m_horizonPosition, 6000, 10, 10);

	//Drawing the clouds
	ofSetColor(255, 255, 255);
	for (ofxAssimpModelLoader cloud : m_clouds) {
		cloud.drawFaces();
	}

	m_light.disable();
	m_cam.end();
	ofDisableDepthTest();
	ofDisableLighting();
}

void ofApp::flapWings() {
	//BELOW only works around it's own Z axis
	//The solution is to make a new node next to the bird, make the wing it's child and then rotate the wing around the parent node


	//if (std::chrono::steady_clock::now() > m_flapTimer) {

		if (m_wingAngle > 45.0f || m_wingAngle < -30.0f) {
			m_wingDirection *= -1.0f;
		}

		//m_wingAngle = ofLerpDegrees(m_wingAngle, 60.0f*m_wingDirection, 0.05f);

		//m_wingAngle = m_wingDirection;

		//m_flapTimer = std::chrono::steady_clock::now() + std::chrono::seconds(1);
	//}


}
void ofApp::tiltCamera(int keyPressed) {
	//Orientation of the coordinate plane looks like:
	//
	/// +deg    	+deg
	// 0 --------cam--------- 180
	/// -deg    	-deg
	//
	//Getting the current rotation
	ofVec3f orientation = m_cam.getOrientationEulerDeg();

	if (keyPressed == OF_KEY_UP) {
		if (orientation.x >= 45.0f) {
			m_cameraTilt = 0.0f;
			m_heightModifier = -20.0f;
		}else {
			//Interpolating between tilting values to make the movement smooth
			m_cameraTilt = ofLerpDegrees(m_cameraTilt, TILT_TARGET, LERP_PERCENT);
			//Adjusting the height of the clouds to simlutate camera movement
			if (orientation.x < 5.0f && orientation.x > -5.0) {
				m_heightModifier = 0.0f;
			}
			else {
				m_heightModifier -= HEIGHT_MOD_TARGET;
			}
		}



	}
	else if (keyPressed == OF_KEY_DOWN) {
		//Setting a mininum value the bird camera can tilt
		if (orientation.x <= -45.0f) {
			m_cameraTilt = 0.0f;
			m_heightModifier = 20.0f;
		}else {
			//Interpolating between tilting values to make the movement smooth
			m_cameraTilt = ofLerpDegrees(m_cameraTilt, -TILT_TARGET, 0.5f);
			//Adjusting clounds up
			if (orientation.x < 5.0f && orientation.x > -5.0) {
				m_heightModifier = 0.0f;
			}
			else {
				m_heightModifier += HEIGHT_MOD_TARGET;
			}

		}


	}


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == OF_KEY_UP) {
		tiltCamera(key);
	}
	if (key == OF_KEY_DOWN) {
		tiltCamera(key);
	}
	//We can implement lerping to make the movement smoother later
	//for testing it is easier not to, so we can add that after Leap Motion interaction is done
	//ofLerp(init, end, increment);
	if (key == 'w') {
		m_birdPosition.y += 5.0f;
	}
	if (key == 'a') {
		m_birdPosition.x -= 5.0f;
	}
	if (key == 's') {
		m_birdPosition.y -= 5.0f;
	}
	if (key == 'd') {
		m_birdPosition.x += 5.0f;
	}
	if(key == 'f'){
		flapWings();
	}

}

void ofApp::generateClouds() {
	//Randomly Keep generating objects that move around the screen.

	if (std::chrono::steady_clock::now() > m_timer)
	{

		ofVec3f randomPosition = {static_cast<float>(rand() % 4000 - 2000), static_cast<float>(rand() % 4000 - 2000), -6000.0f};
		float randomSize = rand() % 50 + 10;

		ofSetColor(255, 255, 255);
		ofFill();

		m_timer = std::chrono::steady_clock::now() + std::chrono::seconds(5);


		ofxAssimpModelLoader cloudModel;
		cloudModel.loadModel("low_poly_cloud_v1.stl");
		cloudModel.setRotation(0, 180, 1, 0, 0);
		cloudModel.setPosition(randomPosition.x,randomPosition.y, randomPosition.z);
		m_clouds.push_back(cloudModel);
	}

}

void ofApp::deleteClouds() {

	//Two variables are used because vectors need iterators, not ints to index positions to remove.
	//However, iterators can't compare values to eachother when they have the type ofAssimpModelLoader
	std::vector<ofxAssimpModelLoader>::iterator it = m_clouds.begin();
	int checkIterator = 0;
	//Clearing coulds that are not used
	for (ofxAssimpModelLoader cloud : m_clouds) {
		if (cloud.getPosition().z > 0) {
			m_clouds.erase(it);
			cout << m_clouds.size() << endl;
			//Don't move iterator
			break;
		}

		//Checking if at the end of the list
		if (checkIterator != m_clouds.size()) {
			//Incrementing both iterators
			advance(it, 1);
			checkIterator++;
		}
		else {
			break;
		}

	}


}
