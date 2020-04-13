#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#endif // INSTRUMENT_H

#include<string>

class Instrument {
    private:
        std::string name;
        float harmonics[9];

    public:
        Instrument(std::string name, float *harmonics);
        std::string getName();
        float *getHarmonics();

};
