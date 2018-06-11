#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(239);
	ofSetWindowTitle("Insta");

	this->font_size = 600;
	this->font.loadFont("fonts/NotoSansCJKjp-Bold.otf", this->font_size, true, true, true);
	this->prev_charactor = '$';

	this->fbo.allocate(ofGetWidth(), ofGetHeight());	
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->charactor = ((ofGetFrameNum() / 60) % 26) + 'A';
	if (this->charactor != this->prev_charactor) {

		string word = string{ this->charactor };
		vector<ofPath> chara_path = this->font.getStringAsPoints(word, true, false);
		this->outline = chara_path[0].getOutline();
		this->word_size = ofPoint(this->font.stringWidth(word), this->font.stringHeight(word));

		this->fbo.begin();
		ofClear(0);

		ofSetColor(0);
		ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

		this->font.drawString(word, -this->word_size.x * 0.5, this->word_size.y * 0.5);

		this->fbo.end();
		this->fbo.readToPixels(this->pix);

		this->prev_charactor = this->charactor;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	int number_of_char = 400;
	int char_count = 0;
	
	while (char_count < number_of_char) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());

		ofColor pix_color = this->pix.getColor(x, y);
		if(pix_color != ofColor(0)){

			continue;
		}

		float x_increase = ofMap(ofNoise(ofRandom(number_of_char), ofGetFrameNum() * 0.005), 0, 1, -20, 20);
		float y_increase = ofMap(ofNoise(ofRandom(number_of_char), ofGetFrameNum() * 0.005), 0, 1, -20, 20);
		x += x_increase;
		y += y_increase;

		ofPushMatrix();
		ofTranslate(x, y);
		ofRotate(ofRandom(360) + x_increase + y_increase);

		float size = ofRandom(3, 50);

		ofColor color;
		color.setHsb(ofRandom(255), 239, 239);
		ofSetColor(color);

		float magnification = size * (1 / this->font_size);

		ofBeginShape();
		for (int line_index = 0; line_index < outline.size(); line_index++) {

			if (line_index != 0) {

				ofNextContour(true);
			}

			vector<glm::vec3> vertices = outline[line_index].getVertices();
			for (int i = 0; i < vertices.size(); i++) {

				ofPoint point(vertices[i].x, vertices[i].y, vertices[i].z);
				point = point - ofPoint(this->word_size.x * 0.5, this->word_size.y * -0.5);
				ofVertex(point * magnification);
			}
		}
		ofEndShape(true);
		ofPopMatrix();

		char_count++;
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}