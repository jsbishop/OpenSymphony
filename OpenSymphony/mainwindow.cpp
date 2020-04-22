#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "track.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include <iostream>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

//#include <QApplication>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#include <../Lib/site-packages/numpy/core/include/numpy/arrayobject.h>
#pragma pop_macro("slots")

#include "instrumentbank.h"

#include <QDebug>

#define BUFSIZE 256

int MainWindow::init_numpy(){
    Py_Initialize();
    import_array(); // PyError if not successful
    //Up to python should be the same as your python path.  After that, include the paths as shown here to site-packages, Lib, and DLLs with a ; between each.
    wchar_t path[] = L"../OpenSymphonyPython/;"
                     "C:/Users/amjas/AppData/Local/Programs/Python/Python38/Lib/site-packages/;"
                     "C:/Users/amjas/AppData/Local/Programs/Python/Python38/Lib/;"
                     "C:/Users/amjas/AppData/Local/Programs/Python/Python38/DLLs";
    PySys_SetPath(path);
    return 0;
}

static int numpy_initialized =  -1;


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), testTrack(this),
	ui(new Ui::MainWindow)
{
	qDebug("I'm here");
	
	ui->setupUi(this);
	
	this->connect(this->ui->actionNew_Project, SIGNAL(triggered(bool)), this, SLOT(newProject()));
	this->connect(this->ui->actionSave_Project, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
	this->connect(this->ui->actionSave_Project_As, SIGNAL(triggered(bool)), this, SLOT(saveProjectAs()));
	this->connect(this->ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openProject()));
	
	
	this->connect(this->ui->actionNew_Track, SIGNAL(triggered(bool)), this, SLOT(createNewTrack()));
    this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(testFourier()));
//    this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(pythonTest()));
//    this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(pythonTestArray()));
	
	this->connect(this->ui->actionSamples, SIGNAL(triggered(bool)), this, SLOT(editTrackSamples()));
	this->connect(this->ui->actionExport_to_Audio_File, SIGNAL(triggered(bool)), this, SLOT(exportAudio()));
	
	//this->ui->scoreTabs->removeTab(1);	//remove the default tabs
	//this->ui->scoreTabs->removeTab(0);
	
	this->testTrack.score.resize(12);
	addTrackTab(&testTrack);
    //this->ui->testButton->hide();

    //initialize python
    numpy_initialized = init_numpy();

    initializeInstrumentBank();


	

}

void MainWindow::initializeInstrumentBank() {
    qDebug() << "initializing bank";
    float harmonicsATrumpet[] = {-0.00685218, 0.05612199, 0.14330995, 0.35116207, 0.14304661, 0.09330923, 0.04869735, 0.01439044, 0.01011234};
    float harmonicsBTrumpet[] = {0, -0.10147628, -0.27464732, -0.11459945, 0.04761091, 0.04968284, 0.06204205, 0.02730316, 0.02236463};
    std::string name = "trumpet";
    iBank.addInstrument(name, harmonicsATrumpet, harmonicsBTrumpet);

    name = "flute";
    float harmonicsAFlute[] = {-2.69739561e-03, 1.88616648e-03, 1.23666485e-03, 1.48290188e-03, 1.97077024e-03, 1.10740578e-03, 9.65661305e-04, 1.03636680e-03, 5.07227983e-04};
    float harmonicsBFlute[] = {0, -9.28241180e-04, -3.34647383e-04, -1.92460995e-04, 8.42807087e-05, 5.67485833e-05, -2.33652650e-04, 1.30743082e-03, 1.21543135e-03};
    iBank.addInstrument(name, harmonicsAFlute, harmonicsBFlute);

//    name = "oboe";
//    float harmonicsOboe[] = {0.4762, 0.4524, 1.0000, 0.0952, 0.1000, 0.1048, 0.2619, 0.1429, 0.0952};
//    iBank.addInstrument(name, harmonicsOboe);

    name = "clarinet";
    float harmonicsAClarinet[] = {-0.00495463, 0.62264983, 0.05649881, 0.11911196, 0.0769048,  0.01987596, 0.04918726, -0.01030753, 0.00503813};
    float harmonicsBClarinet[] = {0, -0.15558717, -0.00952552, 0.40183322, -0.01012529, -0.02638546, -0.04298626, -0.07061608, 0.00396756};
    iBank.addInstrument(name, harmonicsAClarinet, harmonicsBClarinet);

//    name = "guitar";
//    float harmonicsGuitar[] = {0.8000, 0.5440, 1.0000, 0.8800, 0.8800, 0.8000, 0, 0.0400, 0.1600};
//    iBank.addInstrument(name, harmonicsGuitar);

    name = "horn";
    float harmonicsAHorn[] = {-0.00737609, 0.14897398, 0.2757954, 0.14180712, 0.08327578, 0.04651521, 0.0372388, 0.03654996, 0.04140227};
    float harmonicsBHorn[] = {0, -0.07861147, -0.01587843, 0.0676121, 0.02883301, 0.01873752, 0.0073565, -0.00420913, -0.00697062};
    iBank.addInstrument(name, harmonicsAHorn, harmonicsBHorn);

//    name = "piano";
//    float harmonicsPiano[] = {1, .1, .325, .5, .4, .4, 0, .25, 0};
//    iBank.addInstrument(name, harmonicsPiano);

    name = "saxophone";
    float harmonicsASax[] = {0.9896193771626298, 0.05096753581366033, 0.10013420571553104, 0.018186518134140853, 0.046093662169530494, 0.05668130679206973, -0.009909504350049543, 0.016954063175117797, 0.0756057121670712, 0.05270328057459335};
    float harmonicsBSax[] = {0.0, -0.13183625907905278, 0.07795715628868859, 0.005465820394904325, -0.026496233507552803, 0.0064371210718658, -0.02038212454426651, -0.04710743425732519, -0.030681560459952805, 0.026831454243190986};
    iBank.addInstrument(name, harmonicsASax, harmonicsBSax);

    name = "violin";
    float harmonicsAViolin[] = {-0.01898906, 0.28246865, 0.34773771, 0.01329275, 0.01234224, 0.1656276, 0.10546611, 0.00097408, 0.02409484};
    float harmonicsBViolin[] = {0, 0.02152184, -0.0457324, 0.02897899, 0.03525851, -0.05855355, 0.01701746, 0.02169501, 0.01144296};
    iBank.addInstrument(name, harmonicsAViolin, harmonicsBViolin);

    name = "trombone";
    float harmonicsATrombone[] = {-9.63209302e-04, -1.52543762e-02, 6.60848277e-02, -7.59366949e-02, 6.60827194e-02, 1.62116037e-01, 1.27810233e-01, 5.58277808e-02, 7.56025117e-02};
    float harmonicsBTrombone[] = {0, -4.96430326e-02, 3.24357387e-02, -1.02082822e-01, 1.27797127e-04, -5.07751474e-02, -3.42982872e-02, -1.47231716e-02, -1.61552741e-02};
    iBank.addInstrument(name, harmonicsATrombone, harmonicsBTrombone);

    qDebug() << "Base number of instruments: " << iBank.getNumInst();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::newProject() {
	int ret = QMessageBox::question(this, "OpenSymphony", "Do you want to open in a new window?",
								   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	switch(ret) {
		case QMessageBox::Yes:
			//open new project in new window
			//for now I won't try making that work
			break;
			
		case QMessageBox::No:
			//ask user if they want to save current project
			break;
			
		case QMessageBox::Cancel:
			return;
			break;
			
		default:
			return;
	}
	
	//QString newFileName = QFileDialog::getSaveFileName(nullptr, "Create New Project","", "OpenSymphony Project (*.txt)","OpenSymphony Project (*.txt)");
}

void MainWindow::openProject() {
	//if user has unsaved changes to the current project, open in a new window
	int ret = QMessageBox::question(this, "OpenSymphony", "Do you want to save the current project first?",
								   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	qDebug("here");
	switch(ret) {
		case QMessageBox::Yes:
			break;
			
		case QMessageBox::No:
			break;
			
		default:
			return;
	}
	
	QString openFileName = QFileDialog::getOpenFileName(this, "Open Project", "", "OpenSymphony Project (*.txt)");
	
	//this->song.reset(openFileName);
	//set current project to the data loaded from file
	
}

void MainWindow::saveProject() {
	
}

void MainWindow::saveProjectAs() {
//    QString saveFileName = QFileDialog::getSaveFileName(this,"Save Project As...","",tr("OpenSymphony Project (*.txt) *.mid"));
//    switch(this->song.projectFile.setProjectFileName(saveFileName)) {
//        case 0:
//            //do nothing, it succeeded
//            return;
//            break;
			
//        default:
//            int success = QMessageBox::warning(this, "Uh-Oh", "I wasn't able to save the project. Do you want to try again?",
//                                                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
//            switch (success) {
//                case QMessageBox::Yes:
//                    saveProjectAs();
//                    break;
//                default:
//                    return;
//            }
//    }
}


void MainWindow::createNewTrack() {
	Track *newTrack = new Track(nullptr);
	newTrack->show();
	qDebug("I've called newTrack()");
	
	//this->connect(newTrack, SIGNAL(sig_done(Track*)), this->song, SLOT(addTrack(Track*)));
	this->connect(newTrack, SIGNAL(sig_done(Track*)), this, SLOT(addTrackTab(Track*))); 
	
}

void MainWindow::addTrackTab(Track *newTrack) {
	this->song.addTrack(newTrack);
	
	//create a new tab widget
	QTabWidget tempTab;
	
	//add a table to it
	qDebug() << "score length:" << newTrack->score.length();
	
	QTableWidget *scoreGrid = new QTableWidget(20,newTrack->score.length(),nullptr);
	
	QStringList rowLabels = {"A","Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B", "C", "Db", "D", "Eb", "E"};
	
	scoreGrid->setVerticalHeaderLabels(rowLabels);

	QStringList colLabels;
	
	double dt=1.0/16.0;
	for (int jj=0; jj < newTrack->score.length(); jj++) {
		QString tmpstr = QString::number(1.0+dt*jj);
		colLabels.append(tmpstr);
	}
	
	scoreGrid->setHorizontalHeaderLabels(colLabels);
	
	//scoreGrids->setRowCount(20);	
	//scoreGrid->setColumnCount(newTrack->score.length());
	
	//add the tab widget to scoreTabs and send signal to reference table
	//const QString trackName = newTrack->name;
	
	//int index = this->ui->scoreTabs->addTab(scoreGrid, "trackName");
	//QLabel *myLabel = new QLabel("Hello World", this);
	//int index = this->ui->scoreTabs->addTab(myLabel, "My Label Tab");
	//qDebug() << "the index is" << index << "and the table is" << scoreGrid->rowCount() << "x" << scoreGrid->columnCount();
	//adjustGridCheckboxes(scoreGrid);

	int index = this->ui->scoreTabs->addTab(scoreGrid, "Alex's Custom Instrument");
	//qDebug() << "the index is" << index << "and the table is" << scoreGrid->rowCount() << "x" << scoreGrid->columnCount();
	adjustGridCheckboxes(scoreGrid);

	this->ui->scoreTabs->tabBar()->setCurrentIndex(index);
	this->ui->scoreTabs->tabBar()->setTabText(0, "Piano");
	this->ui->scoreTabs->tabBar()->setTabText(1, "Alto Sax");
	this->connect(scoreGrid,SIGNAL(cellClicked(int,int)), this, SLOT(cellChecked(int, int)));

}

void MainWindow::adjustGridCheckboxes(QTableWidget *t) { //automatically fill each empty cell of the table with a checkbox 
	QCheckBox *qb;
	//qDebug() << "height of t is" << t->rowCount();
	for (int row = 0; row < t->rowCount(); row++) {
		for (int col = 0; col < t->columnCount(); col++) {
//			qDebug() << t->itemAt(row,col)->data(Qt::DisplayRole);
			//if (t->itemAt(row,col)->data(Qt::DisplayRole).isNull()) { //need to make sure this will actually return null when it's supposed to
				//qb = new QCheckBox(t->itemAt(row,col));
				t->setItem(row, col, new QTableWidgetItem);
				
				
				QHBoxLayout *l = new QHBoxLayout();
				//QCheckBox *qb = new QCheckBox("");
				//QIcon defaultButtonIcon(":/images/button_default.png");
				//qb->setIcon(defaultButtonIcon);
				//qb->setStyleSheet("QCheckBox::indicator  { width:150px; height: 150px;} QCheckBox::indicator::checked {image: url(:/images/button_checked.png);}");
				//l->addWidget((new QCheckBox("")));
				//l->addWidget(qb);
				
				QWidget *w = new QWidget();
			    w->setLayout(l);
				
				t->setCellWidget(row,col, w);
				//qDebug() << "I did it supposedly for cell [" << row << col << "]";
			//}
		}
	}
	currentTable = t;
}

void MainWindow::adjustGridTimeSignature(int time1, int time2) { //adjust grid to match the appropriate time signature (time1/time2)
	//add time1 cells per measure
	
	//not sure if we need to do anything with the bottom number here
}

void MainWindow::adjustGridLength(int length) { //adjust grid to match the appropriate length
	//length should denote the number of 
	//default for now should be one cell for each beat for the length of the song
	for (int i = 0; i < this->song.tracks.length(); i++) {
		this->song.tracks[i]->setLength(length);		
	}
	//later on will want to have it be multiplied by some number so that the number of cells varies depending on how much the table is zoomed in
}

void MainWindow::cellChecked(int row, int col) { //add note to score
	//determine current tab
	int currentTab = this->ui->scoreTabs->currentIndex();
	
	if (currentTable->item(row,col)->background() == Qt::blue) { //call cellUnchecked if the cell was already blue
		cellUnchecked(row, col);
		return;
	}
	currentTable->item(row,col)->setBackground(Qt::blue);
	
	//add note to score
	qDebug() << "Adding note to track" << currentTab << "at position" << col << "with pitch" << row;	
	//qDebug() << "The size of tracks is " << this->song.tracks.size();
	this->song.addNote(currentTab, col, row);
	
}

void MainWindow::cellUnchecked(int row, int col) { //remove note from score
	qDebug("cellUnchecked");
	//determine current tab
	int currentTab = this->ui->scoreTabs->currentIndex();
	
	//remove note from score
	qDebug() << "Removing note from track" << currentTab << "at position" << col << "with pitch" << row;
	this->song.removeNote(currentTab,col,row);
	currentTable->item(row,col)->setBackground(Qt::white);
	
}

void MainWindow::editTrackSamples() {
	//open the newtrack.ui window corresponding to the current tab (track)
	
//	this->song.tracks[this->ui->scoreTabs->currentIndex()].show();
	//this->song.tracks[this->ui->scoreTabs->currentIndex()].ui.buttonSave.show();
	
}

void MainWindow::exportAudio() {//save to wav file
	QString fileName = QFileDialog::getSaveFileName(this, "Save As Audio File", "", "WAV Files (*.wav)");
	
	//startpulses is an array of time positions indicating when the notes start; can be determined by looking at the score for each track
	float **startPulses;
	startPulses = new float*[this->song.tracks.size()];
	
	float **durations;
	durations = new float*[this->song.tracks.size()];
	
	for (int i = 0; i < this->song.tracks.size(); i++) { //the second dimension should be the length of each track
		startPulses[i] = new float[this->song.tracks[0]->score.length()];	
		durations[i] = new float[this->song.tracks[0]->score.length()];
	}
	
	int startPulses_index = 0;
	for (int i = 0; i < this->song.tracks.size(); i++) {
		for (int j = 0; j < this->song.tracks[0]->score.length(); j++) {
			if (this->song.tracks[i]->score[j].pitch != -1) { //this is assuming for now that every note is going to be a 16th note
				//if the pitch isn't set to null value then it should be considered the starting position of a note
				startPulses[i][startPulses_index] = j;
				
				//durations gonna be hard coded as 1 for now
				
			}
			else {
				startPulses[i][startPulses_index] = -1;
			}
		}
	}
	
	//durations gonna be hard coded as 1 for now
	for (int i = 0; i < this->song.tracks.size(); i++) {
		for (int j = 0; j < this->song.tracks[0]->score.length(); j++) {
			if (this->song.tracks[0]->score[j].pitch == -1) {
				durations[i][j] = -1;				
			}
			else {
				durations[i][j] = 1;
			}
		}
	}
	
	//noteNumbers will be the pitch (row number) for each note in each track (maybe use -1 for empty cells)
	int **noteNumbers;
	for (int i = 0; i < this->song.tracks.size(); i++) {
		for (int j = 0; j < this->song.tracks[0]->score.length(); j++) {
			if (this->song.tracks[0]->score[j].pitch == -1) {
				noteNumbers[i][j] = -1;				
			}
			else {
				noteNumbers[i][j] = this->song.tracks[0]->score[j].pitch;
			}
		}		
	}
	
	//instrumentHarmonicsA/B are hard-coded for the preset instruments, and then what about the custom ones?
	float **instrumentHarmonicsA;
	float **instrumentHarmonicsB;
	//fileName needs to have the extension removed and be converted to char *
	std::string str = fileName.toStdString();
	char* p = new char [str.size()+1];
	strcpy( p, str.c_str() );
	
	//tempo is in bpm, right? Gonna hard code that to 120 for now
	double tempo = 120;
	
	int numVoices = this->song.tracks.size();
	
	int numNotes = this->song.tracks[0]->score.length();
	qDebug() << "about to call writeWav, with numNotes being" << numNotes;
	
	if (writeWav(startPulses, durations, noteNumbers, instrumentHarmonicsA, instrumentHarmonicsB, p, tempo, numVoices, numNotes) == 1) {
		qDebug("I think there was an error trying to write the wav file");
	}
}

//static PyMethodDef PyMethods[] = {
//    {"multiply", multiply, METH_VARARGS},
//    {"mult_array", mult_array, METH_VARARGS},
//    {"mult_array_2d", mult_array_2d, METH_VARARGS},
//    {"string_test", string_test, METH_VARARGS},
//    {NULL, NULL, 0, NULL}
//};

//static struct PyModuleDef py_function =
//{
//    PyModuleDef_HEAD_INIT,
//    "py_function", /* name of module */
//    "",          /* module documentation, may be NULL */
//    -1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
//    PyMethods
//};

//PyMODINIT_FUNC
//PyInitpy_function(void)
//{
//   return PyModule_Create(&py_function);
//}

PyObject *MainWindow::makelist(int array[], size_t size) {
    PyObject *l = PyList_New(size);
    for (size_t i = 0; i != size; ++i) {
        PyList_SET_ITEM(l, i, PyLong_FromLong(array[i]));
    }
    return l;
}

int MainWindow::pythonTest() {
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs, *arglist;
//  char *python_source, *function_name;
    char python_source[] = "py_function";

    //multiply - basic pass arguments test.  2 numbers, multiplies them together
    //mult_array - takes one array, multiplies all contents together
    //string_test - takes one string, returns "Hello " concat with the string
    //mult_array_2d - takes 2d array, no return just prints
    //play - takes 3 2d arrays (starts, durs, notes) + filename(string)
    char function_name[] = "play";

//    wchar_t path[] = L"../OpenSymphonyPython/";
    int args[] = {5, 3};
    int args1[][2] = {{2, 3}, {5, 3}};
    char str[] = "World!";

    // Initialize the Python Interpreter
//    Py_Initialize();
    //numpy
//    if(numpy_initialized != 0) {
//        numpy_initialized = init_numpy();
//    }

//    PySys_SetPath(path);

    // Build the name object
    pName = PyUnicode_FromString(python_source);  //python_source

    // Load the module object
    pModule = PyImport_Import(pName);
//    pModule = PyImport_ImportModule(python_source);
    if(!pModule)
    {
        PyErr_Print();
        return 1;
    }

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);
    if(!pDict)
    {
        PyErr_Print();
        return 1;
    }

    // pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, function_name);  //function_name
    if(!pFunc)
    {
        PyErr_Print();
        return 1;
    }

    if (PyCallable_Check(pFunc))
    {
        // Prepare the argument list for the call
        //multiply
//        pArgs = PyTuple_New(2);

//        for (int i = 0; i < 2; i++)
//        {
//            pValue = PyLong_FromLong(args[i]);
//            if (!pValue)
//            {
//                PyErr_Print();
//                return 1;
//            }

//            PyTuple_SetItem(pArgs, i, pValue);
//        }

//        pArgs = PyTuple_New(1);

        //mult_array
//        pArgs = makelist(args, 2);

        //mult_array2d
//        npy_intp dims[2] = {2, 2};
//        PyTuple_SetItem(pArgs, 0, PyArray_SimpleNewFromData(2, dims, PyArray_LONG, *args1));

//        PyTuple_SetItem(pArgs, 0, arglist);

        //string_test
//        pValue = PyUnicode_FromString(str);
//        PyTuple_SetItem(pArgs, 0, pValue);

//        arglist = Py_BuildValue("(O)", pArgs);

        //play
        int notes[][3] = {{36, 40, 43}, {48, 52, 55}};
        float starts[][3] = {{0, 10, 20}, {0, 10, 20}};
        float durs[][3] = {{30, 20, 10}, {30, 20, 10}};
        char file[] = "../OpenSymphonyPython/wavFiles/testc0.wav";

        pArgs = PyTuple_New(4);
        npy_intp dims[2] = {2, 3};
        PyTuple_SetItem(pArgs, 0, PyArray_SimpleNewFromData(2, dims, PyArray_FLOAT, *starts));
        PyTuple_SetItem(pArgs, 1, PyArray_SimpleNewFromData(2, dims, PyArray_FLOAT, *durs));
        PyTuple_SetItem(pArgs, 2, PyArray_SimpleNewFromData(2, dims, PyArray_LONG, *notes));
        PyTuple_SetItem(pArgs, 3, PyUnicode_FromString(file));

        if(!pArgs)
        {
            PyErr_Print();
            return 1;
        }
        qDebug() << "Pre py func call";

//        pValue = PyObject_CallObject(pFunc, arglist);

        pValue = PyObject_CallObject(pFunc, pArgs);

        if (pArgs != NULL)
        {
//            Py_DECREF(pArgs);
        }
        if (arglist != NULL)
        {
//            Py_DECREF(arglist);
        }

    } else
    {
        PyErr_Print();
    }

    if(pValue != NULL) {
//        qDebug() << "Py return value: " << PyUnicode_AsUTF8(pValue);  //String
        qDebug() << "Py return value: " << PyLong_AsLong(pValue);  //Long
//        Py_DECREF(pValue);
    }

    // Clean up
//    Py_DECREF(pModule);
//    Py_DECREF(pName);
//    Py_DECREF(pFunc);
//    Py_DECREF(pDict);


    qDebug() << "Pre finalize";
    // Finish the Python Interpreter
//    Py_FinalizeEx();

    qDebug() << "End of py func call";
    return 0;
}

/*writeWav
 * pulse = 1/16th note.
 * startPulses = 2d array of pulse values when the notes start for each voice
 * durations = 2d array containing durations of every note for each voice, measured in pulses
 * noteNumbers = 2d array containing the noteNumbers for each voice.
 * Resource: https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
 * use the piano key number
 * instrumentHarmonics A and B = arrays with instrument harmonics.  Should be length 9.
 * filename = desired name of file.  Do not include path or .wav extension.
 * tempo
 * number of voices
 * number of notes - must be uniform for every voice!  Sorry.  Can't send 2D arrays that aren't uniform row length.
 * Suggested hack is to have 0 duration notes to fill space.  Note, a voice can play multiple notes simultaneously.
 * Find max number of notes among all voices
 * Fill the rest of the voices with 0 duration notes until equal. Start pulse and note number won't matter
 * Will return 0 if it worked, 1 if something went wrong that doesn't crash it
 */

int MainWindow::writeWav(float **startPulses, float **durations, int **noteNumbers, float **instrumentHarmonicsA, float **instrumentHarmonicsB, char *filename, double tempo, int numVoices, int numNotes)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs;
    char python_source[] = "openSymphonyPython";
    char function_name[] = "writeWav";

    // Build the name object
    pName = PyUnicode_FromString(python_source);  //python_source

    // Load the module object
    pModule = PyImport_Import(pName);
    if(!pModule)
    {
        PyErr_Print();
        return 1;
    }

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);
    if(!pDict)
    {
        PyErr_Print();
        return 1;
    }

    // pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, function_name);  //function_name
    if(!pFunc)
    {
        PyErr_Print();
        return 1;
    }

    if (PyCallable_Check(pFunc))
    {
        //Create arrays and pointers
        npy_intp dims[2] = {numVoices, numNotes};
        PyObject *startArray = PyArray_SimpleNew(2, dims, PyArray_FLOAT);
        PyObject *durArray = PyArray_SimpleNew(2, dims, PyArray_FLOAT);
        PyObject *noteArray = PyArray_SimpleNew(2, dims, PyArray_LONG);
        float *start_p = (float*) PyArray_DATA(startArray);
        float *dur_p = (float*) PyArray_DATA(durArray);
        int *note_p = (int*) PyArray_DATA(noteArray);

        dims[1] = 9;
        PyObject *harmonicsArrayA = PyArray_SimpleNew(2, dims, PyArray_FLOAT);
        float *harmonicsA_p = (float*) PyArray_DATA(harmonicsArrayA);

        PyObject *harmonicsArrayB = PyArray_SimpleNew(2, dims, PyArray_FLOAT);
        float *harmonicsB_p = (float*) PyArray_DATA(harmonicsArrayB);

        //Copy data into contiguous memory
        for(int i=0; i<numVoices; i++)
        {
            memcpy(start_p, startPulses[i], sizeof(float) * numNotes);
            start_p += numNotes;
            memcpy(dur_p, durations[i], sizeof(float) * numNotes);
            dur_p += numNotes;
            memcpy(note_p, noteNumbers[i], sizeof(int) * numNotes);
            note_p += numNotes;
            memcpy(harmonicsA_p, instrumentHarmonicsA[i], sizeof(float) * 9);
            harmonicsA_p += 9;
            memcpy(harmonicsB_p, instrumentHarmonicsB[i], sizeof(float) * 9);
            harmonicsB_p += 9;
        }

        //Create and fill tuple to pass as argument to python function
        //starts, durs, notes, harmonics, filename, tempo
        //Can add constant volume per voice
        pArgs = PyTuple_New(7);
        PyTuple_SetItem(pArgs, 0, startArray);
        PyTuple_SetItem(pArgs, 1, durArray);
        PyTuple_SetItem(pArgs, 2, noteArray);
        PyTuple_SetItem(pArgs, 3, harmonicsArrayA);
        PyTuple_SetItem(pArgs, 4, harmonicsArrayB);
        PyTuple_SetItem(pArgs, 5, PyUnicode_FromString(filename));
        PyTuple_SetItem(pArgs, 6, PyFloat_FromDouble(tempo));

        if(!pArgs)
        {
            PyErr_Print();
            return 1;
        }

//        qDebug() << "about to call python function";
        pValue = PyObject_CallObject(pFunc, pArgs);
        if(!pValue)
        {
            PyErr_Print();
            return 1;
        }

        int retVal = PyLong_AsLong(pValue);

        return retVal;
    } else
    {
        PyErr_Print();
        return 1;
    }
}

//Use "path/to/file.wav" for argument
//returnvalue[0] is harmonicsA, returnvalue[1] is harmonicsB
float **MainWindow::getFourier(char *filename)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs;
    PyArrayObject *np_ret;
    char python_source[] = "openSymphonyPython";
    char function_name[] = "findHarmonics";
    double *retValue;

    // Build the name object
    pName = PyUnicode_FromString(python_source);  //python_source

    // Load the module object
    pModule = PyImport_Import(pName);
    if(!pModule)
    {
        PyErr_Print();
        return NULL;
    }

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);
    if(!pDict)
    {
        PyErr_Print();
        return NULL;
    }

    // pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, function_name);  //function_name
    if(!pFunc)
    {
        PyErr_Print();
        return NULL;
    }

    if (PyCallable_Check(pFunc))
    {
        pArgs = PyTuple_New(1); // tuple with filename
        pValue = PyUnicode_FromString(filename);
        PyTuple_SetItem(pArgs, 0, pValue);
        pValue = PyObject_CallObject(pFunc, pArgs);
        if(!pValue)
        {
            PyErr_Print();
            return NULL;
        }
        np_ret = reinterpret_cast<PyArrayObject*>(pValue);
        retValue = reinterpret_cast<double*>(PyArray_DATA(np_ret));
        qDebug() << "about to return values";
//        qDebug() << retValue[0];
//        return NULL;
        float **fourier_p = 0;
        fourier_p = new float*[2];
        for(int i=0; i<2; i++)
        {
            fourier_p[i] = new float[9];
            for(int j=0; j<9; j++)
            {
                fourier_p[i][j] = (float) retValue[9*i+j];
//                qDebug() << fourier_p[i][j];
            }
//            qDebug() << retValue[i];
        }
        return fourier_p;

    } else
    {
        PyErr_Print();
        return NULL;
    }
    return NULL;
}

void MainWindow::testFourier()
{
    char filename[] = "../OpenSymphonyPython/wavFiles/TENORSA.wav";
    float **fourier = getFourier(filename);
//    for(int i=0; i<2; i++)
//    {
//        for(int j=0; j<9; j++)
//        {
//            qDebug() << fourier[i][j];
//        }
//    }
}

void MainWindow::testWriteWav()
{
    int notes[][3] = {{36, 40, 43}, {48, 52, 55}};
    float starts[][3] = {{0, 10, 20}, {0, 10, 20}};
    float durs[][3] = {{30, 20, 10}, {30, 20, 10}};
    float *harmonicsA[2];
    float *harmonicsB[2];
    char instrument1[] = "saxophone";
    char instrument2[] = "flute";
    Instrument *sax = iBank.getInstrument(instrument1);
    harmonicsA[0] = sax->getHarmonicsA();
    harmonicsB[0] = sax->getHarmonicsB();
    harmonicsA[1] = sax->getHarmonicsA();
    harmonicsB[1] = sax->getHarmonicsB();
    if(harmonicsA[0] == NULL)
    {
        qDebug() << "find trumpet failed";
        return;
    }
//    harmonicsA[1] = iBank.getInstrument(instrument2)->getHarmonicsA();
    if(harmonicsA[1] == NULL)
    {
        qDebug() << "find flute failed";
        return;
    }

    qDebug() << harmonicsA[0][0];
    qDebug() << harmonicsB[0][0];
//    float harmonicsTrumpet[] = {0.1155, .3417, 0.1789, 0.1232, 0.0678, 0.0473, 0.0260, 0.0045, 0.0020};
//    float harmonicsFlute[] = {0.1111, 1.0000, 0.4000, 0.1944, 0.0444, 0.0111, 0, 0.0111, 0};

//    harmonics[0] = harmonicsTrumpet;
//    harmonics[1] = harmonicsFlute;

    char file[] = "testc3";
    int numVoices = 2;
    int numNotes = 3;
    double tempo = 120;

    float *start_p[2], *dur_p[2];
    int *note_p[2];
    for(int i=0; i<2; i++)
    {
        start_p[i] = starts[i];
        dur_p[i] = durs[i];
        note_p[i] = notes[i];
    }

//    qDebug() << "about to write to file";
    writeWav(start_p, dur_p, note_p, harmonicsA, harmonicsB, file, tempo, numVoices, numNotes);
    qDebug() << "Wrote to file";
}

void MainWindow::pythonTestArray()
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *pArgs;
    PyArrayObject *np_ret;
    char python_source[] = "py_function";
    char function_name[] = "retArray";
    double *retValue;

    // Build the name object
    pName = PyUnicode_FromString(python_source);  //python_source

    // Load the module object
    pModule = PyImport_Import(pName);
    if(!pModule)
    {
        PyErr_Print();
        return;
    }

    // pDict is a borrowed reference
    pDict = PyModule_GetDict(pModule);
    if(!pDict)
    {
        PyErr_Print();
        return;
    }

    // pFunc is also a borrowed reference
    pFunc = PyDict_GetItemString(pDict, function_name);  //function_name
    if(!pFunc)
    {
        PyErr_Print();
        return;
    }

    if (PyCallable_Check(pFunc))
    {
        pArgs = NULL;
        pValue = PyObject_CallObject(pFunc, pArgs);
        if(!pValue)
        {
            PyErr_Print();
            return;
        }
        np_ret = reinterpret_cast<PyArrayObject*>(pValue);
        retValue = reinterpret_cast<double*>(PyArray_DATA(np_ret));
        for(int i=0; i<4; i++) {
            qDebug() << retValue[i];
        }

    } else
    {
        PyErr_Print();
        return;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //Python cleanup?
    qDebug() << "Closing";
    Py_Finalize();
}
