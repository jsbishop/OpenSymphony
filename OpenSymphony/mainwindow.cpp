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
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	qDebug("I'm here");
	
	ui->setupUi(this);
	
	this->connect(this->ui->actionNew_Project, SIGNAL(triggered(bool)), this, SLOT(newProject()));
	this->connect(this->ui->actionSave_Project, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
	this->connect(this->ui->actionSave_Project_As, SIGNAL(triggered(bool)), this, SLOT(saveProjectAs()));
	this->connect(this->ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openProject()));
	
	
	this->connect(this->ui->actionNew_Track, SIGNAL(triggered(bool)), this, SLOT(createNewTrack()));
    this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(testWriteWav()));
//    this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(pythonTest()));
	
	this->connect(this->ui->actionSamples, SIGNAL(triggered(bool)), this, SLOT(editTrackSamples()));
	
	this->testTrack.score.resize(12);
	addTrackTab(&testTrack);
    //this->ui->testButton->hide();

    //initialize python
    numpy_initialized = init_numpy();

//    initializeInstrumentBank();


}

void MainWindow::initializeInstrumentBank() {
    qDebug() << "initializing bank";
    float harmonicsTrumpet[] = {0.1155, .3417, 0.1789, 0.1232, 0.0678, 0.0473, 0.0260, 0.0045, 0.0020};
    std::string name = "trumpet";
    iBank.addInstrument(name, harmonicsTrumpet);
    qDebug() << "trumpet added";
    name = "flute";
    float harmonicsFlute[] = {0.1111, 1.0000, 0.4000, 0.1944, 0.0444, 0.0111, 0, 0.0111, 0};
    iBank.addInstrument(name, harmonicsFlute);

    name = "oboe";
    float harmonicsOboe[] = {0.4762, 0.4524, 1.0000, 0.0952, 0.1000, 0.1048, 0.2619, 0.1429, 0.0952};
    iBank.addInstrument(name, harmonicsOboe);

    name = "clarinet";
    float harmonicsClarinet[] = {1, .275, .225, .1, .3, .2, .1, 0, 0};
    iBank.addInstrument(name, harmonicsClarinet);

    name = "guitar";
    float harmonicsGuitar[] = {0.8000, 0.5440, 1.0000, 0.8800, 0.8800, 0.8000, 0, 0.0400, 0.1600};
    iBank.addInstrument(name, harmonicsGuitar);

    name = "horn";
    float harmonicsHorn[] = {1, .39, .225, .2, .3, .25, .3, .25, .2};
    iBank.addInstrument(name, harmonicsHorn);

    name = "piano";
    float harmonicsPiano[] = {1, .1, .325, .5, .4, .4, 0, .25, 0};
    iBank.addInstrument(name, harmonicsPiano);

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
	Track *newTrack = new Track();
	newTrack->show();
	qDebug("I've called newTrack()");
	
	//this->connect(newTrack, SIGNAL(sig_done(Track*)), this->song, SLOT(addTrack(Track*)));
	this->connect(newTrack, SIGNAL(sig_done(Track*)), this, SLOT(addTrackTab(Track*))); 
	
}

void MainWindow::addTrackTab(Track *newTrack) {
	//create a new tab widget
	QTabWidget tempTab;
	
	//add a table to it
	qDebug() << "score length:" << newTrack->score.length();
	
	QTableWidget *scoreGrid = new QTableWidget(20,newTrack->score.length(),nullptr);
	//scoreGrid->setRowCount(20);	
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
				
				QHBoxLayout *l = new QHBoxLayout();
				QCheckBox *qb = new QCheckBox("");
				QIcon defaultButtonIcon(":/images/button_default.png");
				qb->setIcon(defaultButtonIcon);
				qb->setStyleSheet("QCheckBox::indicator  { width:150px; height: 150px;} QCheckBox::indicator::checked {image: url(:/images/button_checked.png);}");
				//l->addWidget((new QCheckBox("")));
				l->addWidget(qb);
				
				QWidget *w = new QWidget();
			    w->setLayout(l);
				
				t->setCellWidget(row,col, w);
				//qDebug() << "I did it supposedly for cell [" << row << col << "]";
			//}
		}
	}
}

void MainWindow::adjustGridTimeSignature(int time1, int time2) { //adjust grid to match the appropriate time signature (time1/time2)
	//add time1 cells per measure
	
	//not sure if we need to do anything with the bottom number here
}

void MainWindow::adjustGridLength(int length) { //adjust grid to match the appropriate length
	//length should denote the number of 
	//default for now should be one cell for each beat for the length of the song
	
	//later on will want to have it be multiplied by some number so that the number of cells varies depending on how much the table is zoomed in
}

void MainWindow::cellChecked(int row, int col) { //add note to score
	//determine current tab
	int currentTab = this->ui->scoreTabs->currentIndex();
	
	//add note to score
	qDebug() << "Adding note to track" << currentTab << "at position" << col << "with pitch" << row;	
	this->song.addNote(currentTab, col, row);
}

void MainWindow::cellUnchecked(int row, int col) { //remove note from score
	//determine current tab
	int currentTab = this->ui->scoreTabs->currentIndex();
	
	//remove note from score
	qDebug() << "Removing note from track" << currentTab << "at position" << col << "with pitch" << row;
	this->song.removeNote(currentTab,col,row);
}

void MainWindow::editTrackSamples() {
	//open the newtrack.ui window corresponding to the current tab (track)
	
//	this->song.tracks[this->ui->scoreTabs->currentIndex()].show();
	//this->song.tracks[this->ui->scoreTabs->currentIndex()].ui.buttonSave.show();
	
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
 * instrumentHarmonics = array with instrument harmonics.  Should be length 9.
 * filename = desired name of file.  Do not include path or .wav extension.
 * tempo
 * number of voices
 * number of notes - must be uniform for every voice!  Sorry.  Can't send 2D arrays that aren't uniform row length.
 * Suggested hack is to have 0 duration notes to fill space.  Note, a voice can play multiple notes simultaneously.
 * Find max number of notes among all voices
 * Fill the rest of the voices with 0 duration notes until equal. Start pulse and note number won't matter
 * Will return 0 if it worked, 1 if something went wrong that doesn't crash it
 */
int MainWindow::writeWav(float **startPulses, float **durations, int **noteNumbers, float **instrumentHarmonics, char *filename, double tempo, int numVoices, int numNotes)
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
        //Create and fill tuple to pass as argument to python function
        //starts, durs, notes, harmonics, filename, tempo
        //Can add constant volume per voice
        pArgs = PyTuple_New(6);
        npy_intp dims[2] = {numVoices, numNotes};
        PyTuple_SetItem(pArgs, 0, PyArray_SimpleNewFromData(2, dims, PyArray_FLOAT, *startPulses));
        PyTuple_SetItem(pArgs, 1, PyArray_SimpleNewFromData(2, dims, PyArray_FLOAT, *durations));
        PyTuple_SetItem(pArgs, 2, PyArray_SimpleNewFromData(2, dims, PyArray_LONG, *noteNumbers));
        dims[1] = 9;
        PyTuple_SetItem(pArgs, 3, PyArray_SimpleNewFromData(2, dims, PyArray_FLOAT, *instrumentHarmonics));
        PyTuple_SetItem(pArgs, 4, PyUnicode_FromString(filename));
        PyTuple_SetItem(pArgs, 5, PyFloat_FromDouble(tempo));

        if(!pArgs)
        {
            PyErr_Print();
            return 1;
        }

        qDebug() << "about to call python function";
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

void MainWindow::testWriteWav()
{
    qDebug() << "Here0";
    int notes[][3] = {{36, 40, 43}, {48, 52, 55}};
    float starts[][3] = {{0, 10, 20}, {0, 10, 20}};
    float durs[][3] = {{30, 20, 10}, {30, 20, 10}};
    float *harmonics[2];
    char instrument1[] = "trumpet";
    char instrument2[] = "flute";
//    harmonics[0] = iBank.getInstrument(instrument1)->getHarmonics();
//    if(harmonics[0] == NULL)
//    {
//        qDebug() << "find trumpet failed";
//        return;
//    }
//    harmonics[1] = iBank.getInstrument(instrument2)->getHarmonics();
//    if(harmonics[1] == NULL)
//    {
//        qDebug() << "find flute failed";
//        return;
//    }

    float harmonicsTrumpet[] = {0.1155, .3417, 0.1789, 0.1232, 0.0678, 0.0473, 0.0260, 0.0045, 0.0020};
    float harmonicsFlute[] = {0.1111, 1.0000, 0.4000, 0.1944, 0.0444, 0.0111, 0, 0.0111, 0};

    harmonics[0] = harmonicsTrumpet;
    harmonics[1] = harmonicsFlute;

    qDebug() << "Here1";
    char file[] = "testc1";
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

    qDebug() << "about to write to file";
    writeWav(start_p, dur_p, note_p, harmonics, file, tempo, numVoices, numNotes);
    qDebug() << "Wrote to file";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //Python cleanup?
    qDebug() << "Closing";
    Py_Finalize();
}
