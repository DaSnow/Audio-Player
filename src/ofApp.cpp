#include "ofApp.hpp"
#include <filesystem>
#include <string>

using namespace std;
namespace fs = std::filesystem;

//--------------------------------------------------------------
void ofApp::setup()
{
    fs::path p = fs::current_path();
    p += "\\bin\\data";
    for (const auto &dir_entry : fs::directory_iterator{p})
    {
        string filename = dir_entry.path().filename().string();
        string extension = filename.substr(filename.find_last_of("."));
        if (extension == ".wav" || extension == ".mp3")
        {
            songs.push_back(dir_entry.path().filename().string());
        }
    }
    sound.load(songs[0]);               // Loads a sound file (in bin/data/)
    sound.setLoop(loopy);               // Makes the song loop indefinitely
    sound.setVolume(vol);               // Sets the song volume
    ofSetBackgroundColor(255, 170, 80); // Sets the Background Color
}

//--------------------------------------------------------------
void ofApp::update()
{
    /* The update method is called muliple times per second
    It's in charge of updating variables and the logic of our app */
    ofSoundUpdate();               // Updates all sound players
    visualizer.updateAmplitudes(); // Updates Amplitudes for visualizer
    progress = sound.getPosition();
    if (progress >= .99 && !loopy && playL)
    {
        sound.unload();
        selectedSong += (selectedSong < songs.size() - 1) ? 1 : -1 * (songs.size() - 1);
        sound.load(songs[selectedSong]);
        sound.play();
    }
    if (!loopy && !playL)
    {
        playing = !playing;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    /* The update method is called muliple times per second
    It's in charge of drawing all figures and text on screen */

    // Progress Bar
    float progressWidth = ofGetWidth() * progress;
    float progressBarHeight = -50;
    float progressBarY = ofGetHeight();
    float progressBarX = 0;

    ofDrawRectangle(progressBarX, progressBarY, progressWidth, progressBarHeight);
    ofSetColor(256);
    ofFill();

    float pos = playing ? progress : lastPos;
    int percent = pos * 100;
    ofDrawBitmapString("Song Progress: " + ofToString(percent) + "%", 0, 30);
    ofDrawBitmapString("Current Song: " + songs[selectedSong], 0, 45);
    ofDrawBitmapString("Volume: " + to_string(int(vol * 100)) + '%', 0, 60);

    // Mode Selection
    if (!playing)
    {
        ofDrawBitmapString("Press 'p' to play some music!", ofGetWidth() / 2 - 50, ofGetHeight() / 2);
    }
    if (pause)
    {
        ofDrawBitmapString("Paused", ofGetWidth() / 2 - 50, ofGetHeight() / 2);
    }
    vector<float> amplitudes = visualizer.getAmplitudes();
    if (mode == '1')
    {
        drawMode1(amplitudes);
    }
    else if (mode == '2')
    {
        drawMode2(amplitudes);
    }
    else if (mode == '3')
    {
        drawMode3(amplitudes);
    }

    // ofDrawBitmapString("Current Mouse Position: " + ofToString(cur_x) + ", " + ofToString(cur_y), 0, 30);
}
void ofApp::drawMode1(vector<float> amplitudes)
{
    ofFill();        // Drawn Shapes will be filled in with color
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Rectangle Height Visualizer", 0, 15);
    if (!pause)
    {
        lastColorRect = ofRandom(0, 256);
        ofSetColor(0, 0, lastColorRect);
        for (int i = 0; i < amplitudes.size(); i++)
        {
            ofDrawRectRounded((ofGetWidth() / 64) * (i), ofGetHeight() - 100, (ofGetWidth() / 64), amplitudes[i], 10);
        }
    }
    else
    {
        ofSetColor(0, 0, lastColorRect);
        for (int i = 0; i < lastAmp.size(); i++)
        {
            ofDrawRectRounded((ofGetWidth() / 64) * (i), ofGetHeight() - 100, (ofGetWidth() / 64), lastAmp[i], 10);
        }
    }
}
void ofApp::drawMode2(vector<float> amplitudes)
{
    ofSetLineWidth(5); // Sets the line width
    ofNoFill();        // Only the outline of shapes will be drawn
    ofSetColor(256);   // This resets the color of the "brush" to white
    ofDrawBitmapString("Circle Radius Visualizer", 0, 15);
    int bands = amplitudes.size();
    for (int i = 0; i < bands; i++)
    {
        ofSetColor((bands - i) * 32 % 256, 18, 144); // Color varies between frequencies
        if (!pause)
        {
            ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, amplitudes[0] / (i + 1));
        }
        else
        {
            ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 2, lastAmp[0] / (i + 1));
        }
    }
}

void ofApp::drawMode3(vector<float> amplitudes)
{
    ofSetColor(256); // This resets the color of the "brush" to white
    ofDrawBitmapString("Rectangle Width Visualizer", 0, 15);
    // YOUR CODE HERE
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // This method is called automatically when any key is pressed
    switch (key)
    {
    case 'p':
        if (playing)
        {
            sound.stop();
            pause = false;
        }
        else
        {
            sound.play();
        }
        playing = !playing;
        break;
    case 'a':
        if (playing)
        {
            pause = !pause;
            sound.setPaused(pause);
            lastAmp = visualizer.getAmplitudes();
        }
        break;
    case 'd':
        sound.unload();
        selectedSong = (selectedSong < songs.size() - 1) ? ++selectedSong : 0;
        sound.load(songs[selectedSong]);
        sound.setLoop(loopy);
        if (playing)
        {
            sound.play();
        }
        pause = false;
        break;
    case 'r':
        loopy = !loopy;
        sound.setLoop(loopy);
        break;
    case 'l':
        playL = !playL;
        break;
    case '-':
        vol -= (vol > 0) ? .1 : 0;
        sound.setVolume(vol);
        break;
    case '+':
        vol += (vol < 1) ? .1 : 0;
        sound.setVolume(vol);
        break;
    case '1':
        mode = '1';
        break;
    case '2':
        mode = '2';
        break;
    case '3':
        mode = '3';
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    cur_x = x;
    cur_y = y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
//--------------------------------------------------------------