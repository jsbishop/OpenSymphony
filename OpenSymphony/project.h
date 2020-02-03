#ifndef PROJECT_H
#define PROJECT_H

#include "track.h"

class Project
{
	public:
		Project();
		
		QVector<Track> tracks;
		
	public slots:
		void addTrack(Track newTrack);
		
	//create variables for tempo, filename
};

#endif // PROJECT_H
