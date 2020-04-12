#ifndef PROJECT_H
#define PROJECT_H

#include "track.h"
#include "osprojectfile.h"

class Project
{
	public:
		Project();
		
		void reset(QString fileName);
		
//		OSProjectFile projectFile;
		
		//QVector<Track> tracks;
		
	public slots:
		void addTrack(Track newTrack);
		
		void addNote(int trackNum, int pos, int pitch);
		void removeNote(int trackNum, int pos, int pitch);
	//create variables for tempo, filename
};

#endif // PROJECT_H
