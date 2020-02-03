#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "track.h"
#include <stdio.h>
#include <iostream>
//#include "engine.h"
//#include "matrix.h"
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	this->connect(this->ui->actionNew_Track, SIGNAL(triggered(bool)), this, SLOT(createNewTrack()));
	this->connect(this->ui->testButton, SIGNAL(clicked(bool)), this, SLOT(callFevalgcd()));
	
	
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createNewTrack() {
	std::cout << "It worked" << std::endl;
	Track newTrack;
	
	newTrack.show();
	
	//this->connect(newTrack, SIGNAL(sig_done(Track)), this->song, SLOT(addTrack(Track)));
	
}
/*
void MainWindow::callFevalgcd() {

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
    matlab::data::TypedArray<int16_t> result = matlabPtr->feval(u"add(a,b)", args);
    int16_t v = result[0];
    std::cout << "Result: " << v << std::endl;
}

*/
