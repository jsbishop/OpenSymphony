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
        static const int MAX_INSTRUMENTS = 50;
        Instrument *bank;
        int numInst;
};

#endif // INSTRUMENTBANK_H
