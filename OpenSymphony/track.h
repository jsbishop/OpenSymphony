#ifndef TRACK_H
#define TRACK_H

#include <QWidget>
#include <QObject>

namespace Ui {
	class NewTrack;
}

struct Note {
	int pitch; // for now let's say the pitch is measured 
	//double length; // for now let's say the length is measured in quarter notes
	int startPos; //location where the note begins
	int endPos;   //location where the note ends
	int velocity; // not sure what the metric for this would be
	
	int afterTouch; //would be nice to implement but not critical
	int vibrato; //would be nice to implement but not critical
	int pitchBend;	 //would be nice to implement but not critical
};

class Track : public QWidget
{
		Q_OBJECT
		
	public:
		Track();
		explicit Track(QWidget *parent = nullptr);
		~Track();
		void setLength(int length);
		void addNote(int pos, int pitch);
		void removeNote(int pos, int pitch);
		
		QString name;
		QVector<Note> score; // a structure that contains the actual notes
				
	signals:
		void sig_done(Track*);
		
	private:
		Ui::NewTrack *ui;
		
};

#endif // TRACK_H
