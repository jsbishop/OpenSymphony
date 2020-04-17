import numpy as np
import math
from scipy.io import wavfile

'''py_function.py - Python source designed to '''
'''demonstrate the use of python embedding'''


def retArray():
    a = np.asarray([[5.5, 1.0], [2.0, 3.0]])
    print(a)
    return a


def multiply(a, b):
    c = 12345*6789
    c = a*b
    print("python multiply check: ", c)
    return c


def mult_array(a):
    c = 1
    for i in a:
        c *= i

    print("Python mult_array check: ", c)
    return c

def mult_array_2d(a):
    c = 1
    # a = np.import_array(a)
    for i in a:
        for j in i:
            c *= j

    print("mult_array_2d result: ", c)
    return c


def string_test(str):
    ret_str = "Hello "

    return ret_str + str


def play(starts, durs, notes, filename, tempo=120):
    voices = []
    starts = np.asarray(starts)
    durs = np.asarray(durs)
    notes = np.asarray(notes)

    if starts.ndim > 1:
        for startA, durA, noteA in zip(starts, durs, notes):
            voices.append({"startPulses": startA, "durations": durA, "noteNumbers": noteA})
    else:
        voices.append({"startPulses": starts, "durations": durs, "noteNumbers": notes})

    # voices = [{"startPulses": [0, 10, 20], "durations": [30, 20, 10], "noteNumbers": [36, 40, 43]},
    #           {"startPulses": [0, 10, 20], "durations": [30, 20, 10], "noteNumbers": [48, 52, 55]}]

    fs = 8000

    # tempo in bpm.  Divide by 60 for bps.  inverse for spb.  4 pulses per beat
    spp = (1/(tempo/60))/4
    maxl = 0


    for voice in voices:
        for startPulse, duration in zip(voice["startPulses"], voice["durations"]):
            temp = startPulse + duration

            if temp > maxl:
                maxl = temp

    song = np.zeros(int(spp*fs*maxl))

    for voice in voices:
        for startPulse, duration, noteNumber in zip(voice["startPulses"], voice["durations"], voice["noteNumbers"]):
            keynum = noteNumber
            freq = 440 * (2**((keynum - 49) / 12.))
            dur = spp*duration
            tt = np.arange(0., dur, (1. / fs), dtype=float)
            E = np.hanning(len(tt))
            z = np.exp(1j*2*math.pi*freq*tt)
            note = np.asarray(E.transpose()*z.real)  # Create sinusoid of correct length to represent a single note
            locstart = int(spp * fs * startPulse)  # Index of where note starts
            locend = int(locstart + len(tt))  # index of where note ends

            song[locstart:locend] = song[locstart:locend] + note

    song = ((song + song.min()) * (2 ** 15) / song.ptp()).astype(np.int16)

    wavfile.write(filename, fs, song)

    return 0

