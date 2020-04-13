#ifndef INSTRUMENTBANK_H
#define INSTRUMENTBANK_H

#include "instrument.h"
#include <stdlib.h>
#include<string>

class InstrumentBank
{
    public:
        InstrumentBank();
        void addInstrument(std::string name, float *harmonics);
        Instrument *getInstrument(std::string name);
        int getNumInst();

    private:
        Instrument *bank = (Instrument*) malloc(100*sizeof(Instrument));
        int numInst;
};

#endif // INSTRUMENTBANK_H
