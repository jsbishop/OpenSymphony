#include "instrument.h"
#include<QtDebug>

Instrument::Instrument()
{
    name = "";
}

Instrument::Instrument(std::string name, float harmonicsA[9], float harmonicsB[9])
{
//    qDebug() << "in instrument constructor";
    this->name = name;
    for(int i=0; i<9; i++)
    {
        this->harmonicsA[i] = harmonicsA[i];
        this->harmonicsB[i] = harmonicsB[i];
    }
//    qDebug() << "exiting constructor";
}

std::string Instrument::getName()
{
    return name;
}

float *Instrument::getHarmonicsA()
{
    return harmonicsA;
}

float *Instrument::getHarmonicsB()
{
    return harmonicsB;
}
