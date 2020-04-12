#include "instrumentbank.h"

InstrumentBank::InstrumentBank()
{
    numInst = 0;
}

void InstrumentBank::addInstrument(char *name, float *harmonics)
{
    Instrument newI (name, harmonics);
    bank[numInst] = newI;
}
