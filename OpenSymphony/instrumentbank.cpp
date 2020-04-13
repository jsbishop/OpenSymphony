#include "instrumentbank.h"
#include <QDebug>
#include<iostream>
#include<algorithm>

InstrumentBank::InstrumentBank()
{
    numInst = 0;
//    bank = (Instrument*) malloc(MAX_INSTRUMENTS*sizeof(Instrument));
    bank = new Instrument[100];
//    std::fill(bank, bank+MAX_INSTRUMENTS, NULL);
}

void InstrumentBank::addInstrument(std::string name, float harmonics[9])
{
//    qDebug() << "in add instrument";
    Instrument newI (name, harmonics);
//    qDebug() << "instrument constructed";
//    qDebug() << newI.getName().c_str();
    try {
        bank[numInst++] = newI;
    } catch(std::exception e)
    {
        qDebug() << e.what();
    }

//    qDebug() << "Instrument added";
}

Instrument *InstrumentBank::getInstrument(std::string name)
{
//    qDebug() << "Looking for: " << name.c_str();
    for(int i=0; i<numInst; i++)
    {
//        std::cout << "getInstrument found: " << bank[i].getName();
        if(bank[i].getName().compare(name) == 0)
        {
//            qDebug() << "Found: " << bank[i].getName().c_str();
            return &bank[i];
        }
    }
    qDebug() << "getInstrument failed";
    return NULL;
}

int InstrumentBank::getNumInst()
{
    return numInst;
}
