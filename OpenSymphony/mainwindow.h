#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTableWidget>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include "track.h"
#include "project.h"

#include "instrumentbank.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		
		Track testTrack;
		QVector<QVector<QTableWidgetItem>> tableCells; //hacky temporary solution for keeping track of cells in the grid
		QTableWidget *currentTable; //points to the currently relevant table	
			
	public slots:
		void newProject();
		void openProject();
		void saveProject();
		void saveProjectAs();
		void exportAudio();
		
		void createNewTrack();
		void addTrackTab(Track *newTrack);
		
		void removeTrack();
		
		void adjustGridCheckboxes(QTableWidget *t);
		void adjustGridTimeSignature(int time1, int time2);
		void adjustGridLength(int length);
		
		void cellChecked(int row, int col);
		void cellUnchecked(int row, int col);
		
        int pythonTest();
        PyObject *makelist(int array[], size_t size);
        int init_numpy();
        int writeWav(float **startPulses, float **durations, int **noteNumbers, float **instrumentHarmonicsA, float **instrumentHarmonicsB, char *filename, double tempo, int numVoices, int numNotes);
        void testWriteWav();
        void pythonTestArray();
        float **getFourier(char *filename);
        void testFourier();
		
		void addNewInstrument(char *name, char *file);		

		void editTrackSamples();

        void closeEvent(QCloseEvent *event);

        void initializeInstrumentBank();
		
		
	private:
		Ui::MainWindow *ui;
		
		Project song;
		
//        OSProjectFile OSPF;
		
		QVector<Track *> tracks; //this will store the information for each of the tracks, including maybe the actual notes

        InstrumentBank iBank;
		
		int songLength;
};

//class GridBox : public QItemDelegate {
		
//}
#endif // MAINWINDOW_H
