#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "track.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
//#include "mat.h"
#include <iostream>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
//#include "engine.h"
//#include "matrix.h"

//#include "MatlabEngine/matlab_engine.hpp"

//#include "MatlabDataArray.hpp"
//#include "MatlabEngine.hpp"

//#include "PythonQt.h"
//#include <QApplication>

#pragma push_macro("slots")
#undef slots
#ifdef _DEBUG
  #undef _DEBUG
  #include <Python.h>
  #define _DEBUG
#else
  #include <Python.h>
#endif
#pragma pop_macro("slots")

#include <QDebug>

#define BUFSIZE 256


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	qDebug("I'm here");
	
	ui->setupUi(this);
	
	this->connect(this->ui->actionSave_Project, SIGNAL(triggered(bool)), this, SLOT(saveProject()));
	this->connect(this->ui->actionSave_Project_As, SIGNAL(triggered(bool)), this, SLOT(saveProjectAs()));
	
	
	this->connect(this->ui->actionNew_Track, SIGNAL(triggered(bool)), this, SLOT(createNewTrack()));
	//this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(callFevalgcd()));
	this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(aFunction()));
	
	this->connect(this->ui->actionSamples, SIGNAL(triggered(bool)), this, SLOT(editTrackSamples()));
	
	Track testTrack;
	testTrack.score.resize(12);
	addTrackTab(&testTrack);
	
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
	
	QString newFileName = QFileDialog::getSaveFileName(this->ui, "Create New Project","", "OpenSymphony Project (*.txt)","OpenSymphony Project (*.txt)");
}

void MainWindow::openProject() {
	//if user has unsaved changes to the current project, open in a new window
	int ret = QMessageBox::question(this, "OpenSymphony", "Do you want to save the current project first?",
								   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	switch(ret) {
		case QMessageBox::Yes:
			break;
			
		case QMessageBox::No:
			break;
			
		default:
			return;
	}
	
	QString openFileName = QFileDialog::getOpenFileName(this, "Open Project", "", "OpenSymphony Project (*.txt)");
	
	this->song.reset(openFileName);
	//set current project to the data loaded from file
	
}

void MainWindow::saveProject() {
	
}

void MainWindow::saveProjectAs() {
	QString saveFileName = QFileDialog::getSaveFileName(this,"Save Project As...","","*.midi, OpenSymphony Project (*.txt)","OpenSymphony Project (*.txt)");
	
}


void MainWindow::createNewTrack() {
	std::cout << "It worked" << std::endl;
	Track newTrack;
	
	newTrack.show();
	
//	this->connect(newTrack, SIGNAL(sig_done(Track*)), this->song, SLOT(addTrack(Track*)));
//	this->connect(newTrack, SIGNAL(sig_done(Track*)), this, SLOT(addTrackTab(Track*))); 
	
}

void MainWindow::addTrackTab(Track *newTrack) {
	//create a new tab widget
	QTabWidget tempTab;
	
	//add a table to it
	QTableWidget scoreGrid(&tempTab);
	scoreGrid.setColumnCount(newTrack->score.length());
	
	
	//add the tab widget to scoreTabs and send signal to reference table
	const QString trackName = newTrack->name;
	
	this->ui->scoreTabs->addTab(&tempTab, trackName);
	adjustGridCheckboxes(&scoreGrid);

}

void MainWindow::adjustGridCheckboxes(QTableWidget *t) { //automatically fill each empty cell of the table with a checkbox 
	QCheckBox *qb;
	
	for (int row = 0; row < t->height(); row++) {
		for (int col = 0; col < t->width(); col++) {
//			qDebug() << t->itemAt(row,col)->data(Qt::DisplayRole);
			//if (t->itemAt(row,col)->data(Qt::DisplayRole).isNull()) { //need to make sure this will actually return null when it's supposed to
				//qb = new QCheckBox(t->itemAt(row,col));
				
				QHBoxLayout *l = new QHBoxLayout();
				l->addWidget((new QCheckBox()));
				
				QWidget *w = new QWidget();
			    w->setLayout(l);
				
				t->setCellWidget(row,col, w);
				qDebug("I did it supposedly");
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


void MainWindow::editTrackSamples() {
	//open the newtrack.ui window corresponding to the current tab (track)
	
	this->song.tracks[this->ui->scoreTabs->currentIndex()].show();
	this->song.tracks[this->ui->scoreTabs->currentIndex()].ui.buttonSave.show();
	
}

void MainWindow::callFevalgcd() {
/*
    // Pass vector containing MATLAB data array scalar
    using namespace matlab::engine;

    // Start MATLAB engine synchronously
    std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

    // Create MATLAB data array factory
    matlab::data::ArrayFactory factory;

    // Pass vector containing 2 scalar args in vector    
    std::vector<matlab::data::Array> args({
        factory.createScalar<int16_t>(30),
        factory.createScalar<int16_t>(56) });

    // Call MATLAB function and return result
    FutureResult<matlab::data::Array> future = matlabPtr->fevalAsync(u"gcd", args);
	matlab::data::TypedArray<int16_t> results = future.get();
    int16_t v = results[0];
    std::cout << "Result: " << v << std::endl;
 */
}
 

int MainWindow::aFunction() {
//      MATFile *pmat;
//	  mxArray *pa1, *pa2, *pa3;
//	  std::vector<int> myInts;
//	  myInts.push_back(1);
//	  myInts.push_back(2);
//	  printf("Accessing a STL vector: %d\n", myInts[1]);
	
//	  double data[9] = { 1.0, 4.0, 7.0, 2.0, 5.0, 8.0, 3.0, 6.0, 9.0 };
//	  const char *file = "tempSong.mat";
//	  char str[BUFSIZE];
//	  int status;
	
//	  printf("Creating file %s...\n\n", file);
//	  pmat = matOpen(file, "w");
//	  if (pmat == NULL) {
//		printf("Error creating file %s\n", file);
//		printf("(Do you have write permission in this directory?)\n");
//		return(EXIT_FAILURE);
//	  }
	
//	  pa1 = mxCreateDoubleMatrix(3,3,mxREAL);
//	  if (pa1 == NULL) {
//		  printf("%s : Out of memory on line %d\n", __FILE__, __LINE__);
//		  printf("Unable to create mxArray.\n");
//		  return(EXIT_FAILURE);
//	  }
	
//	  pa2 = mxCreateDoubleMatrix(3,3,mxREAL);
//	  if (pa2 == NULL) {
//		  printf("%s : Out of memory on line %d\n", __FILE__, __LINE__);
//		  printf("Unable to create mxArray.\n");
//		  return(EXIT_FAILURE);
//	  }
//	  memcpy((void *)(mxGetPr(pa2)), (void *)data, sizeof(data));
	  
//	  pa3 = mxCreateString("MATLAB: the language of technical computing");
//	  if (pa3 == NULL) {
//		  printf("%s :  Out of memory on line %d\n", __FILE__, __LINE__);
//		  printf("Unable to create string mxArray.\n");
//		  return(EXIT_FAILURE);
//	  }
	
//	  status = matPutVariable(pmat, "LocalDouble", pa1);
//	  if (status != 0) {
//		  printf("%s :  Error using matPutVariable on line %d\n", __FILE__, __LINE__);
//		  return(EXIT_FAILURE);
//	  }
	  
//	  status = matPutVariableAsGlobal(pmat, "GlobalDouble", pa2);
//	  if (status != 0) {
//		  printf("Error using matPutVariableAsGlobal\n");
//		  return(EXIT_FAILURE);
//	  }
	  
//	  status = matPutVariable(pmat, "LocalString", pa3);
//	  if (status != 0) {
//		  printf("%s :  Error using matPutVariable on line %d\n", __FILE__, __LINE__);
//		  return(EXIT_FAILURE);
//	  }
	  
//	  /*
//	   * Ooops! we need to copy data before writing the array.  (Well,
//	   * ok, this was really intentional.) This demonstrates that
//	   * matPutVariable will overwrite an existing array in a MAT-file.
//	   */
//	  memcpy((void *)(mxGetPr(pa1)), (void *)data, sizeof(data));
//	  status = matPutVariable(pmat, "LocalDouble", pa1);
//	  if (status != 0) {
//		  printf("%s :  Error using matPutVariable on line %d\n", __FILE__, __LINE__);
//		  return(EXIT_FAILURE);
//	  }
	  
//	  /* clean up */
//	  mxDestroyArray(pa1);
//	  mxDestroyArray(pa2);
//	  mxDestroyArray(pa3);
	
//	  if (matClose(pmat) != 0) {
//		printf("Error closing file %s\n",file);
//		return(EXIT_FAILURE);
//	  }
	
//	  /*
//	   * Re-open file and verify its contents with matGetVariable
//	   */
//	  pmat = matOpen(file, "r");
//	  if (pmat == NULL) {
//		printf("Error reopening file %s\n", file);
//		return(EXIT_FAILURE);
//	  }
	
//	  /*
//	   * Read in each array we just wrote
//	   */
//	  pa1 = matGetVariable(pmat, "LocalDouble");
//	  if (pa1 == NULL) {
//		printf("Error reading existing matrix LocalDouble\n");
//		return(EXIT_FAILURE);
//	  }
//	  if (mxGetNumberOfDimensions(pa1) != 2) {
//		printf("Error saving matrix: result does not have two dimensions\n");
//		return(EXIT_FAILURE);
//	  }
	
//	  pa2 = matGetVariable(pmat, "GlobalDouble");
//	  if (pa2 == NULL) {
//		printf("Error reading existing matrix GlobalDouble\n");
//		return(EXIT_FAILURE);
//	  }
//	  if (!(mxIsFromGlobalWS(pa2))) {
//		printf("Error saving global matrix: result is not global\n");
//		return(EXIT_FAILURE);
//	  }
	
//	  pa3 = matGetVariable(pmat, "LocalString");
//	  if (pa3 == NULL) {
//		printf("Error reading existing matrix LocalString\n");
//		return(EXIT_FAILURE);
//	  }
	  
//	  status = mxGetString(pa3, str, sizeof(str));
//	  if(status != 0) {
//		  printf("Not enough space. String is truncated.");
//		  return(EXIT_FAILURE);
//	  }
//	  if (strcmp(str, "MATLAB: the language of technical computing")) {
//		printf("Error saving string: result has incorrect contents\n");
//		return(EXIT_FAILURE);
//	  }
	
//	  /* clean up before exit */
//	  mxDestroyArray(pa1);
//	  mxDestroyArray(pa2);
//	  mxDestroyArray(pa3);
	
//	  if (matClose(pmat) != 0) {
//		printf("Error closing file %s\n",file);
//		return(EXIT_FAILURE);
//	  }
//	  printf("Done\n");
//	  return(EXIT_SUCCESS);

      PyObject *pName, *pModule, *pDict, *pFunc, *pValue;
//      char *python_source, *function_name;
      char python_source[] = "../MATLAB/py_function";
      char function_name[] = "multiply";
//       if (argc < 3)
//       {
//           printf("Usage: exe_name python_source function_name\n");
//           return 1;
//       }

       // Initialize the Python Interpreter
      Py_Initialize();

      // Build the name object
      pName = PyUnicode_FromString(python_source);  //python_source

      // Load the module object
      pModule = PyImport_Import(pName);

      // pDict is a borrowed reference
      pDict = PyModule_GetDict(pModule);

      // pFunc is also a borrowed reference
      pFunc = PyDict_GetItemString(pDict, function_name);  //function_name

      if (PyCallable_Check(pFunc))
      {
          PyObject_CallObject(pFunc, NULL);
      } else
      {
          PyErr_Print();
      }

      // Clean up
      Py_DECREF(pModule);
      Py_DECREF(pName);

      // Finish the Python Interpreter
      Py_Finalize();
      return 0;
}

