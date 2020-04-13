#include "instrumentbank.h"
#include <QDebug>
#include<iostream>

InstrumentBank::InstrumentBank()
{
    numInst = 0;
}

void InstrumentBank::addInstrument(std::string name, float harmonics[9])
{
    qDebug() << "in add instrument";
    Instrument newI (name, harmonics);
    qDebug() << "instrument constructed";
    qDebug() << newI.getName().c_str();
    bank[numInst++] = newI;
    qDebug() << "Instrument added";
}

Instrument *InstrumentBank::getInstrument(std::string name)
{
    qDebug() << "getInstrument";
    for(int i=0; i<numInst; i++)
    {
        std::cout << "getInstrument found: " << bank[i].getName();
        if(bank[i].getName().compare(name))
        {
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
