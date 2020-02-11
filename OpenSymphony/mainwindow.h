#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTableWidget>
#include "track.h"
#include "project.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		
	public slots:
		void createNewTrack();
		void addTrackTab(Track *newTrack);
		
		void adjustGridCheckboxes(QTableWidget *t);
		void adjustGridTimeSignature(int time1, int time2);
		void adjustGridLength(int length);
		
		void callFevalgcd();
		int aFunction();
		
		
		
	private:
		Ui::MainWindow *ui;
		
		Project song;
		
		QVector<Track> tracks; //this will store the information for each of the tracks, including maybe the actual notes
};

#endif // MAINWINDOW_H
