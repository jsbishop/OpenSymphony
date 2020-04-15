import numpy as np
from scipy.io import wavfile
import math


def writeWav(starts, durs, notes, harmonics, filename, tempo=120):
    voices = []
    starts = np.asarray(starts)
    durs = np.asarray(durs)
    notes = np.asarray(notes)

    if starts.ndim > 1:
        for startA, durA, noteA, harmonicsA in zip(starts, durs, notes, harmonics):
            voices.append({"startPulses": startA, "durations": durA, "noteNumbers": noteA, "harmonics": harmonicsA})
    else:
        voices.append({"startPulses": starts, "durations": durs, "noteNumbers": notes, "harmonics": harmonics})

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
        Ak = voice["harmonics"]
        for startPulse, duration, noteNumber in zip(voice["startPulses"], voice["durations"], voice["noteNumbers"]):
            keynum = noteNumber
            freq = 440 * (2**((keynum - 49) / 12.))
            dur = spp*duration
            tt = np.arange(0., dur, (1. / fs), dtype=float)

            # Create sinusoid of correct length to represent a single note
            note = np.asarray(Ak[0] * np.cos(2 * math.pi * 1 * freq * tt))
            for j in range(1, len(Ak)):
                note = note + Ak[j] * np.cos(2 * math.pi * (j + 1) * freq * tt)
            locstart = int(spp * fs * startPulse)  # Index of where note starts
            locend = int(locstart + len(tt))  # index of where note ends

            song[locstart:locend] = song[locstart:locend] + note

    song = ((song + song.min()) * (2 ** 15) / song.ptp()).astype(np.int16)

    path = "../OpenSymphonyPython/wavFiles/"

    file = path + filename + ".wav"

    wavfile.write(file, fs, song)

    return 0


# https://dspillustrations.com/pages/posts/misc/the-sound-of-harmonics-approximating-instrument-sounds-with-fourier-series.html - resource
def fourierSeries(period, N):
    """Calculate the Fourier series coefficients up to the Nth harmonic"""
    result = []
    T = len(period)
    t = np.arange(T)
    for n in range(N+1):
        an = 2/T*(period * np.cos(2*np.pi*n*t/T)).sum()
        bn = 2/T*(period * np.sin(2*np.pi*n*t/T)).sum()
        result.append((an, bn))
    return np.array(result)


def extractPeriod(data, rate):
    maxA = max(data[len(data)//2:])
    print(max(data), maxA)
    found = 0
    t_start = 0
    t_end = 0
    for i in range(len(data)):
        if data[i] >= .95*maxA and found == 0:
            t_start = i
            found = 1
        elif data[i] >= .95*maxA and found == 1:
            t_end = i
            found = 2

    print(t_start, t_end)
    sample_start = int(t_start * rate)
    sample_end = int(t_end*rate)
    # print(sample_start, sample_end)
    period = data[t_start:t_end]

    # print(len(data), len(period))

    return period, rate


def findHarmonics(filename):
    path = "../OpenSymphonyPython/wavFiles/"
    file = path + filename
    rate, data = wavfile.read(filename)

    period, rate = extractPeriod(data, rate)
    fSeries = fourierSeries(period, 9)
    fSet = fSeries[0]/max(fSeries[0])

    return fSeries


def reconstruct(P, anbn):
    """Sum up sines and cosines according to the coefficients to
    produce a reconstruction of the original waveform"""
    result = 0
    t = np.arange(P)
    for n, (a, b) in enumerate(anbn):
        if n == 0:
            a = a/2
        result = result + a*np.cos(2*np.pi*n*t/P) + b * np.sin(2*np.pi*n*t/P)
    return result

def synthesizeTest():
    notes = [[36, 40, 43], [48, 52, 55]]
    starts = [[0, 10, 20], [0, 10, 20]]
    durs = [[30, 20, 10], [30, 20, 10]]
    tempo = 120
    rate, data = wavfile.read("wavfiles/TENORSA.wav")
    data = data * 1.0 / (abs(data).max())
    if(data.ndim > 1):
        data = data.sum(axis=1) / 2
        period, rate = extractPeriod(data, rate)
    else:
        period, rate = extractPeriod(data, rate)

    anbn = fourierSeries(period, 9)
    print(anbn)
    voices = []
    starts = np.asarray(starts)
    durs = np.asarray(durs)
    notes = np.asarray(notes)

    if starts.ndim > 1:
        for startA, durA, noteA in zip(starts, durs, notes):
            voices.append({"startPulses": startA, "durations": durA, "noteNumbers": noteA})
    else:
        voices.append({"startPulses": starts, "durations": durs, "noteNumbers": notes})

    fs = 11100

    # tempo in bpm.  Divide by 60 for bps.  inverse for spb.  4 pulses per beat
    spp = (1 / (tempo / 60)) / 4
    maxl = 0

    for voice in voices:
        for startPulse, duration in zip(voice["startPulses"], voice["durations"]):
            temp = startPulse + duration

            if temp > maxl:
                maxl = temp

    song = np.zeros(int(spp * fs * maxl))

    for voice in voices:
        for startPulse, duration, noteNumber in zip(voice["startPulses"], voice["durations"], voice["noteNumbers"]):
            keynum = noteNumber
            freq = 440 * (2 ** ((keynum - 49) / 12.))
            # P = int((1./freq)*fs)
            P = len(period)
            dur = spp * duration
            tt = np.arange(0., dur, (1. / fs), dtype=float)

            note = 0

            for n, (a, b) in enumerate(anbn):
                if n == 0:
                    a = a / 2
                note = note + a * np.cos(2 * np.pi * n * tt * freq) + b * np.sin(2 * np.pi * n * tt * freq)

            locstart = int(spp * fs * startPulse)  # Index of where note starts
            locend = int(locstart + len(tt))  # index of where note ends

            song[locstart:locend] = song[locstart:locend] + note

    return song, fs




# testing function
if __name__ == '__main__':
    # notes = [[36, 40, 43], [48, 52, 55]]
    # start = [[0, 10, 20], [0, 10, 20]]
    # dur = [[30, 20, 10], [30, 20, 10]]
    # Ak = [[0.1111, 1.0000, 0.4000, 0.1944, 0.0444, 0.0111, 0, 0.0111, 0],
    #       [0.1155, .3417, 0.1789, 0.1232, 0.0678, 0.0473, 0.0260, 0.0045, 0.0020]]
    # file = "testFromPy"
    #
    # writeWav(start, dur, notes, Ak, file)

    # fSeries = findHarmonics("wavFiles/test_flute.wav")
    # print(fSeries)

    song, fs = synthesizeTest()

    # song = ((song + song.min()) * (2 ** 15) / song.ptp()).astype(np.int16)
    # song = song * 1.0 / (abs(song).max())
    song = song.astype(np.float32)
    print(song)
    wavfile.write("wavFiles/test_new0.wav", fs, song)

