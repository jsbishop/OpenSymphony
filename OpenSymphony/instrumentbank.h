#ifndef INSTRUMENTBANK_H
#define INSTRUMENTBANK_H

#include "instrument.h"
#include <stdlib.h>

class InstrumentBank
{
    public:
        InstrumentBank();
        void addInstrument(char *name, float *harmonics);

    private:
        Instrument *bank = (Instrument*) malloc(100*sizeof(Instrument));
        int numInst;
};

#endif // INSTRUMENTBANK_H
