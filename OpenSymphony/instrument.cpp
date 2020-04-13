#include "instrument.h"
#include<QtDebug>

Instrument::Instrument()
{
    name = "";
}

Instrument::Instrument(std::string name, float harmonics[9])
{
//    qDebug() << "in instrument constructor";
    this->name = name;
    for(int i=0; i<9; i++)
    {
        this->harmonics[i] = harmonics[i];
    }
//    qDebug() << "exiting constructor";
}

std::string Instrument::getName()
{
    return name;
}

float *Instrument::getHarmonics()
{
    return harmonics;
}
