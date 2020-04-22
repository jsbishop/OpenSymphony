#include "track.h"
#include "ui_newtrack.h"

Track::Track() {
	
}
Track::Track(QWidget *parent) : QWidget(parent), ui(new Ui::NewTrack) {
	ui->setupUi(this);
//	this->setModal(true);
	
	qDebug("created new track");
}

Track::~Track()
{
	delete ui;
}


void on_buttonDone_clicked() {
	//add track to project
//	emit this->sig_done(&this);	 //probably gonna complain cos it's passing a pointer instead of the value
}

void Track::setLength(int length) {
	this->score.resize(length);
}

void Track::addNote(int pos, int pitch) {
	if (pos >= this->score.length()) {
		this->score.resize(pos);
	}
	this->score[pos].pitch = pitch;
	
}

void Track::removeNote(int pos, int pitch) { //reset note parameters to default values
	//int subdivider = 4;					//**change this later on once you can obtain the number of notes per beat**

	for (int i = 0; i < this->score.length(); i++) {
		if (this->score[i].startPos <= pos && this->score[i].endPos >= pos && this->score[i].pitch == pitch) {
			this->score.removeAt(i);
		}
	}
}

