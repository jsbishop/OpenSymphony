#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "track.h"
#include <stdio.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	this->connect(this->ui->actionNew_Track, SIGNAL(triggered(bool)), this, SLOT(createNewTrack()));
	
	
	
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createNewTrack() {
	std::cout << "It worked" << std::endl;
	Track newTrack;
	
	newTrack.show();
	
	this->connect(newTrack, SIGNAL(sig_done(Track)), this->song, SLOT(addTrack(Track)));
	
}

