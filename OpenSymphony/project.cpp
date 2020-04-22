#include "project.h"
#include "track.h"
#include <QObject>
#include <QDebug>

Project::Project()
{
	//this->tracks = new QVector<Track>;
	//this->tracks.resize(1);
}

void Project::addTrack(Track *newTrack) {
	this->tracks.push_back(newTrack);
	if (this->tracks[this->tracks.size()-1] == nullptr) {
		qDebug () << "it's also nullptr here";
	}
}

void Project::addNote(int trackNum, int pos, int pitch) {
	if(this->tracks[trackNum-2] == nullptr) {
		qDebug() << "it's nullptr and tracknum is" <<trackNum << "and the size of tracks is" << this->tracks.size();
	}
	this->tracks[trackNum-2]->addNote(pos, pitch);
}

void Project::removeNote(int trackNum, int pos, int pitch) {
	this->tracks[trackNum-2]->removeNote(pos, pitch);
}
