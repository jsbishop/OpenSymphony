#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
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
		void callFevalgcd();
		int aFunction();
		
		
	private:
		Ui::MainWindow *ui;
		
		Project song;
		
		QVector<Track> tracks; //this will store the information for each of the tracks, including maybe the actual notes
};

#endif // MAINWINDOW_H
