#ifndef TRACK_H
#define TRACK_H

#include <QWidget>
#include <QObject>

namespace Ui {
	class NewTrack;
}

struct Note {
	int pitch; // for now let's say the pitch is measured 
	double length; // for now let's say the length is measured in quarter notes
	int velocity; // not sure what the metric for this would be
	
	int afterTouch; //would be nice to implement but not critical
	int vibrato; //would be nice to implement but not critical
	int pitchBend;	 //would be nice to implement but not critical
};

class Track : public QWidget
{
		Q_OBJECT
		
	public:
		explicit Track(QWidget *parent = 0);
		~Track();
		QString name;
		
	signals:
		void sig_done(Track*);
		
	private:
		Ui::NewTrack *ui;
		QVector<Note> Score; // a structure that contains the actual notes
		
};

#endif // TRACK_H
