/************************************************************
************************************************************/
/* */
#include "ofApp.h"

/* for dir search */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h> 
#include <dirent.h>
#include <string>

using namespace std;


/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp()
: id(0)
, f_ContentsChange(CONTENTS_CHANGE__NONE)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
	printMessage("Exit");

	for(int i = 0; i < Table_mov.size(); i++){
		printf("[%5d] %3d:%s\n", i, Table_mov[i].b_check, Table_mov[i].FileName.c_str());
	}
}

/******************************
******************************/
void ofApp::exit()
{
}

//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	ReadConfig();
	
	/********************
	********************/
	ofSetWindowTitle("Hap Player");
	
	ofSetWindowShape( MONITOR_WIDTH, MONITOR_HEIGHT );
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	makeup_mov_table(path_mov, Table_mov);
	
	/********************
	********************/
	printMessage("Load");
	printf("%s\n", Table_mov[id].FileName.c_str());
	video.load(Table_mov[id].FileName.c_str());
	setup_video(video);
}

/******************************
******************************/
void ofApp::setup_video(ofxHapPlayer& video)
{
	video.setLoopState(OF_LOOP_NORMAL);
	// video.setLoopState(OF_LOOP_PALINDROME);
	
	video.setSpeed(1);
	video.setVolume(0.0);
	video.play();
}

/******************************
******************************/
void ofApp::ReadConfig()
{
	/********************
	********************/
	FILE* fp;
	fp = fopen("../../../data/config.txt", "r");
	if(fp == NULL) { ERROR_MSG(); std::exit(1); }
	
	char buf[BUF_SIZE];
	while(fscanf(fp, "%s", buf) != EOF){
		if(strcmp(buf, "<mov_path>") == 0){
			/********************
			スキャン集合
				http://wisdom.sakura.ne.jp/programming/c/c58.html
			********************/
			fscanf(fp, "%[ \t]", buf); // space & tab 読み捨て
			fscanf(fp, "%[^\n]", buf); // \n以外を読み取る -> \nが来るまで読み込み(space also)
			sprintf(path_mov, "%s", buf);
		}
	}
	
	fclose(fp);
	
	/********************
	********************/
	printMessage("config data");
	printf("path_mov :%s\n", path_mov);
}

/******************************
******************************/
void ofApp::makeup_mov_table(const string dirname, vector<TABLE_MOV_INFO>& Table_mov)
{
	/********************
	********************/
	DIR *pDir;
	struct dirent *pEnt;
	struct stat wStat;
	string wPathName;

	pDir = opendir( dirname.c_str() );
	if ( NULL == pDir ) { ERROR_MSG(); std::exit(1); }

	pEnt = readdir( pDir );
	while ( pEnt ) {
		// .と..は処理しない
		if ( strcmp( pEnt->d_name, "." ) && strcmp( pEnt->d_name, ".." ) ) {
		
			wPathName = dirname + "/" + pEnt->d_name;
			
			// ファイルの情報を取得
			if ( stat( wPathName.c_str(), &wStat ) ) {
				printf( "Failed to get stat %s \n", wPathName.c_str() );
				break;
			}
			
			if ( S_ISDIR( wStat.st_mode ) ) {
				// nothing.
			} else {
			
				vector<string> str = ofSplitString(pEnt->d_name, ".");
				if(str[str.size()-1] == "mov"){
					TABLE_MOV_INFO Table_NewVal;
					
					Table_NewVal.FileName = wPathName;
					
					vector<string> str2 = ofSplitString(pEnt->d_name, "#");
					if(str2.size() == 3){
						Table_NewVal.BeatInterval_ms = atoi(str2[1].c_str());
					}else{
						Table_NewVal.BeatInterval_ms = -1;
					}
					
					Table_mov.push_back(Table_NewVal);
				}
			}
		}
		
		pEnt = readdir( pDir ); // 次のファイルを検索する
	}

	closedir( pDir );
}

//--------------------------------------------------------------
void ofApp::update(){

	/********************
	********************/
	if( (f_ContentsChange == CONTENTS_CHANGE__NEXT) || (f_ContentsChange == CONTENTS_CHANGE__PREV) ){
		ChangeVideoContents();
		f_ContentsChange = CONTENTS_CHANGE__NONE;
	}
	
	/********************
	********************/
	video.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	/********************
	********************/
	ofBackground(0);
	ofSetColor(255, 255, 255, 255);
	
	video.draw(0, 0, ofGetWidth(), ofGetHeight());
}

/******************************
******************************/
int ofApp::getNextId_Table_mov(vector<TABLE_MOV_INFO>& Table_mov, int& id)
{
	id++;
	if(Table_mov.size() <= id){
		id = 0;
	}
	
	return id;
}

/******************************
******************************/
int ofApp::getPrevId_Table_mov(vector<TABLE_MOV_INFO>& Table_mov, int& id)
{
	id--;
	if(id < 0){
		id = Table_mov.size() - 1;
	}
	
	return id;
}

/******************************
******************************/
void ofApp::ChangeVideoContents()
{
	/********************
	********************/
	video.stop();
	video.close();
	
	/********************
	********************/
	if(f_ContentsChange == CONTENTS_CHANGE__NEXT){
		id = getNextId_Table_mov(Table_mov, id);
	}else if(f_ContentsChange == CONTENTS_CHANGE__PREV){
		id = getPrevId_Table_mov(Table_mov, id);
	}
	
	printf("[%5d] %s\n", id, Table_mov[id].FileName.c_str());
	video.load(Table_mov[id].FileName.c_str());
	setup_video(video);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case OF_KEY_UP:
			f_ContentsChange = CONTENTS_CHANGE__PREV;
			break;
			
		case OF_KEY_DOWN:
			f_ContentsChange = CONTENTS_CHANGE__NEXT;
			break;
			
		case 'd':
			Table_mov[id].b_check = false;
			break;
			
		case ' ':
			Table_mov[id].b_check = true;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
