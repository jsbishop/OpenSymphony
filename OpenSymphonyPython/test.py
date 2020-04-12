import numpy as np
import scipy
from scipy.io import wavfile
import pydub
from pydub import AudioSegment
import math

AudioSegment.converter = r"D:\\ffmpeg\\bin"


def test():
    sampleRate = 44100
    frequency = 440
    length = 5

    t = np.linspace(0, length, sampleRate * length)  # Produces a 5 second Audio-File
    y = np.sin(frequency * 2 * np.pi * t)  # Has frequency of 440Hz
    y = ((y + y.min()) * (2 ** 15) / y.ptp()).astype(np.int16)

    wavfile.write('wavFiles/test.wav', sampleRate, y)


def play(starts, durs, notes):
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

    spp = .15
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

    return song


def play_trumpet():
    voices = []
    notes = np.asarray([[36, 40, 43], [48, 52, 55]])
    starts = np.asarray([[0, 10, 20], [0, 10, 20]])
    durs = np.asarray([[30, 20, 10], [30, 20, 10]])

    if starts.ndim > 1:
        for startA, durA, noteA in zip(starts, durs, notes):
            voices.append({"startPulses": startA, "durations": durA, "noteNumbers": noteA})
    else:
        voices.append({"startPulses": starts, "durations": durs, "noteNumbers": notes})

    # voices = [{"startPulses": [0, 10, 20], "durations": [30, 20, 10], "noteNumbers": [36, 40, 43]},
    #           {"startPulses": [0, 10, 20], "durations": [30, 20, 10], "noteNumbers": [48, 52, 55]}]

    fs = 8000

    spp = .15
    maxl = 0

    # trumpet
    # Ak = [0.1155, .3417, 0.1789, 0.1232, 0.0678, 0.0473, 0.0260, 0.0045, 0.0020]
    # phik = [-2.1299, 1.6727, -2.5454, .6607, -2.0390, 2.1597, -1.0467, 1.8581, -2.3925]

    # experiment - flute?
    # Ak = [0.1111, 1.0000, 0.4000, 0.1944, 0.0444, 0.0111, 0, 0.0111, 0]

    # oboe?
    Ak = [0.4762, 0.4524, 1.0000, 0.0952, 0.1000, 0.1048, 0.2619, 0.1429, 0.0952]

    # clarinet?
    Ak = [1, .275, .225, .1, .3, .2, .1, 0, 0]

    # horn?
    Ak = [1, .39, .225, .2, .3, .25, .3, .25, .2, .1, .05]

    # guitar?
    Ak = [0.8000, 0.5440, 1.0000, 0.8800, 0.8800, 0.8000, 0, 0.0400, 0.1600, 0.6000]

    # piano?
    Ak = [1, .1, .325, .5, .4, .4, 0, .25, 0]

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
            # E = np.hanning(len(tt))
            z = np.exp(1j*2*math.pi*freq*tt)
            # note = np.asarray(Ak[0]*np.cos(np.cos(freq*1*tt-phik[0])))  # Create sinusoid of correct length to represent a single note
            note = np.asarray(Ak[0] * np.cos(2 * math.pi * 1 * freq * tt))
            for j in range(1, len(Ak)):
                note = note + Ak[j] * np.cos(2 * math.pi * (j+1) * freq * tt)
                # note = note + Ak[j] * np.cos(np.cos(freq * (j+1) * tt - phik[j]))

            locstart = int(spp * fs * startPulse)  # Index of where note starts
            locend = int(locstart + len(tt))  # index of where note ends

            song[locstart:locend] = song[locstart:locend] + note

    return song


if __name__ == '__main__':
    # notes = [[36, 40, 43], [48, 52, 55]]
    # start = [[0, 10, 20], [0, 10, 20]]
    # dur = [[30, 20, 10], [30, 20, 10]]

    notes = [36, 40, 43]
    start = [0, 10, 20]
    dur = [30, 20, 10]

    s = play_trumpet()
    s = ((s + s.min()) * (2 ** 15) / s.ptp()).astype(np.int16)

    wavfile.write('wavFiles/test.wav', 8000, s)
    # AudioSegment.from_wav("wavFiles/test.wav").export("mp3Files/test.mp3", format="mp3")


