#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#endif // INSTRUMENT_H

#include<string>

class Instrument {
    private:
        std::string name;
        float harmonicsA[9];
        float harmonicsB[9];

    public:
        Instrument();
        Instrument(std::string name, float *harmonicsA, float *harmonicsB);
        std::string getName();
        float *getHarmonicsA();
        float *getHarmonicsB();

};
