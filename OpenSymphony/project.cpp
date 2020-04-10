#include "project.h"
#include "track.h"

Project::Project()
{
	//this->tracks = new QVector<Track>(1);
//	this->tracks->resize(1);
}

void Project::addTrack(Track newTrack) {
	//this->tracks.push_back(newTrack);
}

void Project::addNote(int trackNum, int pos, int pitch) {
	//this->tracks[trackNum].addNote(pos, pitch);
}

void Project::removeNote(int trackNum, int pos, int pitch) {
	//this->tracks[trackNum].removeNote(pos, pitch);
}
