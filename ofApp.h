/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxHapPlayer.h"

#include "sjCommon.h"

/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
	/*
		MONITOR_WIDTH = 320,
		MONITOR_HEIGHT = 180,
	*/
		MONITOR_WIDTH = 1280,
		MONITOR_HEIGHT = 720,
	};
	enum{
		VIDEO_WIDTH = 1280,
		VIDEO_HEIGHT = 720,
	};
	
	enum CONTENTS_CHANGE{
		CONTENTS_CHANGE__NONE,
		CONTENTS_CHANGE__NEXT,
		CONTENTS_CHANGE__PREV,
	};
	
	/****************************************
	****************************************/
	struct TABLE_MOV_INFO{
		string FileName;
		int BeatInterval_ms;
		
		bool b_check;
		
		TABLE_MOV_INFO()
		: b_check(false)
		{
		}
	};
	
	/****************************************
	****************************************/
	ofxHapPlayer video;
	char path_mov[BUF_SIZE];
	vector<TABLE_MOV_INFO> Table_mov;
	
	int id;
	CONTENTS_CHANGE f_ContentsChange;

public:
	/****************************************
	****************************************/
	/********************
	********************/
	void ReadConfig();
	void makeup_mov_table(const string dirname, vector<TABLE_MOV_INFO>& Table_mov);
	void setup_video(ofxHapPlayer& video);
	int getNextId_Table_mov(vector<TABLE_MOV_INFO>& Table_mov, int& id);
	int getPrevId_Table_mov(vector<TABLE_MOV_INFO>& Table_mov, int& id);
	void ChangeVideoContents();
	
	/********************
	********************/
	ofApp();
	~ofApp();
	
	void exit();
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
	
};
