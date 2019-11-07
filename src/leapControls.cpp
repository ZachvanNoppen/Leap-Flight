#include "leapControls.h"

//--------------------------------------------------------------
void leapControls::setup() {
	m_bird.load("bird_small.png");
	ofSetRectMode(OF_RECTMODE_CENTER);

	ofSetWindowShape(WINDOW_WIDTH, WINDOW_HEIGHT);
	ofSetFrameRate(FRAMERATE);

	m_device.connectEventHandler(&leapControls::onLeapFrame, this);

	// initializations, adjust as need be
	m_birdPos.set(0, -10);
	m_birdVelocity.set(0.0, -1.0);
	m_flapped = false;
	m_birdAngle = 0.0f;
	m_birdMaxVelocity = 15.0f / 32;
	m_birdAcceleration = 0.5f / 32;
	m_wingAngle1 = 0.0f;
	m_wingAngle2 = 0.0f;

}

//--------------------------------------------------------------
void leapControls::update() {

	m_device.update();

	// get the hands that the leap can see
	const Leap::HandList& hands = m_frame.hands();

	// set wing angles
	m_wingAngle2 = (hands.leftmost().grabAngle() * 180.0 / 3.14) + 90;
	m_wingAngle1 = (hands.rightmost().grabAngle() * 180.0 / 3.14) + 90;

	// if the grab angle for both hands is above the set threshold the wings will flap
	if ((hands[0].grabAngle() > 3.14 / 2) && (hands[1].grabAngle() > 3.14 / 2)) {
		if (!m_flapped) {
			m_birdVelocity.y += m_birdMaxVelocity;
			m_flapped = true;
		}
	}
	else m_flapped = false;

	// set the bird's angle based on the palm normal, the normal being a number from -1 to +1
	m_birdAngle = hands[0].palmNormal().x * -90.0f;
	m_birdVelocity.x -= hands[0].palmNormal().x / 32;

	// gravity
	m_birdVelocity.y -= m_birdAcceleration;

	// prevents the bird from going too fast
	if (abs(m_birdVelocity.y) > m_birdMaxVelocity) {
		m_birdVelocity.y = m_birdMaxVelocity * (m_birdVelocity.y / abs(m_birdVelocity.y));
	}
	if (abs(m_birdVelocity.x) > m_birdMaxVelocity / 2) {
		m_birdVelocity.x = m_birdMaxVelocity / 2 * (m_birdVelocity.x / abs(m_birdVelocity.x));
	}

	// prevents the bird from going out of frame
	if (m_birdPos.x > WINDOW_WIDTH / 32) {
		m_birdPos.x = WINDOW_WIDTH / 32;
		m_birdVelocity.x = 0;
	}
	else if (m_birdPos.x < WINDOW_WIDTH / -32) {
		m_birdPos.x = WINDOW_WIDTH / -32;
		m_birdVelocity.x = 0;
	}
	if (m_birdPos.y > (WINDOW_HEIGHT / 32)) {
		m_birdPos.y = WINDOW_HEIGHT / -32;
		m_birdVelocity.set(0, 0);
	}

	//what happens when the bird falls below the screen
	else if (m_birdPos.y < (WINDOW_HEIGHT / -32)) {
		m_birdPos.set(0, 0);
		m_birdVelocity.set(0, 0);
	}

	// modify the position with the velocity
	m_birdPos += m_birdVelocity;

}

//--------------------------------------------------------------
void leapControls::draw() {
	ofPushMatrix();
	ofTranslate(m_birdPos);
	ofRotate(m_birdAngle);
	m_bird.draw(0, 0);
	ofPopMatrix();
}

void leapControls::onLeapFrame(Leap::Frame frame) {
	m_frame = frame;
}