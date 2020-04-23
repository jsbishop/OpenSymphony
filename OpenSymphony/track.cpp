#include "track.h"
#include "ui_newtrack.h"
#include <QObject>
#include <QFileDialog>
#include <QDebug>

Track::Track() {
	
}
Track::Track(QWidget *parent) : QWidget(parent), ui(new Ui::NewTrack) {
	ui->setupUi(this);
//	this->setModal(true);
	
	qDebug("created new track");
	this->isPreset = true;
	this->ui->buttonSave->hide();
}

Track::~Track()
{
	delete ui;
}

void Track::on_buttonBrowse_clicked() {
	qDebug("browse");
	QString fileName = QFileDialog::getOpenFileName(this, "Select Audio Clip", "", "WAV Files (*.wav)");
	if (fileName.isEmpty()) {	
		return;
	}
	
	this->ui->radioBtn_sample->setChecked(true);
	
	this->ui->lineEdit_sample->setText(fileName);
	
	if (!this->ui->lineEdit_trackTitle->text().isEmpty()) {
		this->ui->buttonSave->show();			
	}
}

void Track::on_presetSelection_activated(int index) {
	if (index != 0) {
		this->ui->radioBtn_preset->setChecked(true);
		
		if (!this->ui->lineEdit_trackTitle->text().isEmpty()) {
			this->ui->buttonSave->show();			
		}
	}
	else {
		this->ui->buttonSave->hide();
	}
}

void Track::on_radioBtn_sample_toggled() {
	if (this->ui->radioBtn_sample->isChecked()) {
		this->isPreset = false;
		qDebug("isPreset is now false");
		
	}
	else {
		this->isPreset = true;
		qDebug("isPreset is now true");
	}
}

void Track::on_radioBtn_preset_toggled() {
	//if (this->ui->radioBtn_sample->isChecked()) {
	//	this->isPreset = false;
	//}
}

void Track::on_lineEdit_trackTitle_textChanged(const QString &text) {
	if (text.isEmpty()) {
		this->ui->buttonSave->hide();
	} 
	else if (isPreset && this->ui->presetSelection->currentIndex() != 0) {
		this->ui->buttonSave->show();
	}
	else if (!isPreset && !this->ui->lineEdit_sample->text().isEmpty()) {
		this->ui->buttonSave->show();
	}
}

void Track::on_buttonSave_clicked() {
	this->name = this->ui->lineEdit_trackTitle->text();	
	
	if (this->isPreset) {
		//set the instrument to the preset the user chose
		this->instrumentName = this->ui->presetSelection->currentText();
		qDebug() << "chosen preset is" << instrumentName;
		this->sampleFileName = "";
	}
	
	else {
		this->instrumentName = this->ui->lineEdit_trackTitle->text();
		//create new instrument 
		this->sampleFileName = this->ui->lineEdit_sample->text();
		
	}
	//add track to project
	emit this->sig_done(this);	 //probably gonna complain cos it's passing a pointer instead of the value
	this->close();
}

void Track::on_buttonCancel_clicked() {
	this->close();
}

void Track::setLength(int length) {
	this->score.resize(length);
}

void Track::addNote(int pos, int pitch) {
	if (pos >= this->score.length()) {
		this->score.resize(pos);
	}

	this->score[pos].pitch = pitch;
	qDebug() << "The pitch of the" << this->name << "track at position" << pos << "is" << this->score[pos].pitch << "and it's supposed to be" << pitch;
}

void Track::removeNote(int pos, int pitch) { //reset note parameters to default values
	//int subdivider = 4;					//**change this later on once you can obtain the number of notes per beat**

	for (int i = 0; i < this->score.length(); i++) {
		if (this->score[i].startPos <= pos && this->score[i].endPos >= pos && this->score[i].pitch == pitch) {
			this->score.removeAt(i);
		}
	}
}

