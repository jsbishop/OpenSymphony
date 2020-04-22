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
		QString instrumentName;
		QString sampleFileName;
		bool isPreset;
		QVector<Note> score; // a structure that contains the actual notes
				
	signals:
		void sig_done(Track*);
		
	private:
		Ui::NewTrack *ui;
		
	private slots:
		void on_radioBtn_sample_toggled();
		void on_radioBtn_preset_toggled();
		void on_lineEdit_trackTitle_textChanged(const QString &text);		
		void on_buttonBrowse_clicked();
		void on_presetSelection_activated(int index);		
		void on_buttonSave_clicked();
		
		
};

#endif // TRACK_H
