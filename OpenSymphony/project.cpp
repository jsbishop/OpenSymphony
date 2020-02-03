#include "project.h"
#include "track.h"

Project::Project()
{
	
}

void Project::addTrack(Track newTrack) {
	this->tracks.push_back(newTrack);
}
