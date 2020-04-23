import numpy as np
from scipy.io import wavfile
import math


def writeWav(starts, durs, notes, fSeriesA, fSeriesB, filename, tempo=120):
    voices = []
    starts = np.asarray(starts)
    durs = np.asarray(durs)
    notes = np.asarray(notes)
    print(starts)
    print(durs)
    print(notes)
    harmonics = [[]]
    i = 0
    for harmonicsA, harmonicsB in zip(fSeriesA, fSeriesB):
        for (a, b) in zip(harmonicsA, harmonicsB):
            harmonics[i].append((a, b))

        harmonics.append([])
        i += 1

    harmonics = np.asarray(harmonics)
    # print(harmonics)

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
            # note = np.asarray(Ak[0] * np.cos(2 * math.pi * 1 * freq * tt))
            # for j in range(1, len(Ak)):
            #     note = note + Ak[j] * np.cos(2 * math.pi * (j + 1) * freq * tt)

            note = reconstructFreq(tt, freq, Ak)

            locstart = int(spp * fs * startPulse)  # Index of where note starts
            locend = int(locstart + len(tt))  # index of where note ends

            song[locstart:locend] = song[locstart:locend] + note

    # song = ((song + song.min()) * (2 ** 15) / song.ptp()).astype(np.int16)
    song = song.astype(np.float32)

    path = "../OpenSymphonyPython/wavFiles/"

    # file = path + filename

    wavfile.write(filename, fs, song)

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
    # maxA = max(data[len(data)//2:])
    maxA = max(data)
    # print(max(data), maxA)
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

    # print(t_start, t_end)
    sample_start = int(t_start * rate)
    sample_end = int(t_end*rate)
    # print(sample_start, sample_end)
    period = data[t_start:t_end]

    # print(len(data), len(period))

    return period, rate


def findHarmonics(filename):
    # path = "../OpenSymphonyPython/wavFiles/"
    # file = path + filename
    rate, data = wavfile.read(filename)
    data = data[len(data)//2:]
    data = data * 1.0 / (abs(data).max())
    if len(data.shape) > 1:  # for stereo data, use only first channel
        data = data[:, 0]
    # print(data)
    period, rate = extractPeriod(data, rate)
    fSeries = fourierSeries(period, 8)
    # fSet = fSeries[0]/max(fSeries[0])
    fSeriesA = []
    fSeriesB = []

    for n, (a, b) in enumerate(fSeries):
        fSeriesA.append(a)
        fSeriesB.append(b)

    fSeries = [fSeriesA, fSeriesB]
    fSeries = np.asarray(fSeries)
    # print(fSeries)
    return fSeries


def reconstruct(P, anbn):
    """Sum up sines and cosines according to the coefficients to
    produce a reconstruction of the original waveform"""
    result = 0
    t = np.arange(P*1000)
    for n, (a, b) in enumerate(anbn):
        if n == 0:
            a = a/2
        result = result + a*np.cos(2*np.pi*n*t/P) + b * np.sin(2*np.pi*n*t/P)
    return result


def reconstructFreq(t,freq, anbn):
    """Sum up sines and cosines according to the coefficients to
    produce a reconstruction of the original waveform"""
    result = 0
    # t = np.arange(P*1000)
    # print(anbn)
    for n, (a, b) in enumerate(anbn):
        if n == 0:
            a = a/2
        result = result + a*np.cos(2*np.pi*n*t*freq) + b * np.sin(2*np.pi*n*t*freq)
    return result


def synthesizeTest():
    notes = [[36, 40, 43], [48, 52, 55]]
    starts = [[0, 10, 20], [0, 10, 20]]
    durs = [[30, 20, 10], [30, 20, 10]]
    # notes = [[40]]
    # starts = [[0]]
    # durs = [[30]]
    tempo = 120
    rate, data = wavfile.read("wavfiles/TENORSA.wav")
    data = data * 1.0 / (abs(data).max())
    if(data.ndim > 1):
        data = data.sum(axis=1) / 2
        period, rate = extractPeriod(data, rate)
    else:
        period, rate = extractPeriod(data, rate)

    anbn = fourierSeries(period, 9)
    fSeriesA = [[], []]
    fSeriesB = [[], []]

    for n, (a,b) in enumerate(anbn):
        fSeriesA[0].append(a)
        fSeriesB[0].append(b)
        fSeriesA[1].append(a)
        fSeriesB[1].append(b)

    # print(anbn)
    # print(fSeriesA)
    # print(fSeriesB)
    fSeries = [[]]
    i = 0

    for harmonicsA, harmonicsB in zip(fSeriesA, fSeriesB):
        for(a, b) in zip(harmonicsA, harmonicsB):
            fSeries[i].append((a, b))

        fSeries.append([])
        i += 1

    fSeries = np.asarray(fSeries)
    # print(fSeries)
    voices = []
    starts = np.asarray(starts)
    durs = np.asarray(durs)
    notes = np.asarray(notes)

    if starts.ndim > 1:
        for startA, durA, noteA, harmonics in zip(starts, durs, notes, fSeries):
            voices.append({"startPulses": startA, "durations": durA, "noteNumbers": noteA, "harmonics": harmonics})
    else:
        voices.append({"startPulses": starts, "durations": durs, "noteNumbers": notes, "harmonics": fSeries})

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
        harmonics = voice["harmonics"]
        for startPulse, duration, noteNumber in zip(voice["startPulses"], voice["durations"], voice["noteNumbers"]):
            keynum = noteNumber
            freq = 440 * (2 ** ((keynum - 49) / 12.))
            # P = int((1./freq)*fs)
            P = len(period)
            dur = spp * duration
            tt = np.arange(0., dur, (1. / fs), dtype=float)

            print(harmonics)
            note = reconstructFreq(tt, freq, harmonics)

            # for n, (a, b) in enumerate(anbn):
            #     if n == 0:
            #         a = a / 2
            #     note = note + a * np.cos(2 * np.pi * n * tt * freq) + b * np.sin(2 * np.pi * n * tt * freq)

            locstart = int(spp * fs * startPulse)  # Index of where note starts
            locend = int(locstart + len(tt))  # index of where note ends

            song[locstart:locend] = song[locstart:locend] + note

    return song, fs


# testing function
if __name__ == '__main__':
    notes = [[36, 40, 43], [48, 52, 55]]
    start = [[0, 10, 20], [0, 10, 20]]
    dur = [[30, 20, 10], [30, 20, 10]]
    harmonicsA = [[0.9896193771626298, 0.05096753581366033, 0.10013420571553104, 0.018186518134140853, 0.046093662169530494, 0.05668130679206973, -0.009909504350049543, 0.016954063175117797, 0.0756057121670712, 0.05270328057459335],
    [0.9896193771626298, 0.05096753581366033, 0.10013420571553104, 0.018186518134140853, 0.046093662169530494, 0.05668130679206973, -0.009909504350049543, 0.016954063175117797, 0.0756057121670712, 0.05270328057459335]]
    harmonicsB = [[0.0, -0.13183625907905278, 0.07795715628868859, 0.005465820394904325, -0.026496233507552803, 0.0064371210718658, -0.02038212454426651, -0.04710743425732519, -0.030681560459952805, 0.026831454243190986],
    [0.0, -0.13183625907905278, 0.07795715628868859, 0.005465820394904325, -0.026496233507552803, 0.0064371210718658, -0.02038212454426651, -0.04710743425732519, -0.030681560459952805, 0.026831454243190986]]
    # Ak = [[0.1111, 1.0000, 0.4000, 0.1944, 0.0444, 0.0111, 0, 0.0111, 0],
    #       [0.1155, .3417, 0.1789, 0.1232, 0.0678, 0.0473, 0.0260, 0.0045, 0.0020]]
    file = "testFromPy1"
    #
    writeWav(start, dur, notes, harmonicsA, harmonicsB, file)

    # fSeries = findHarmonics("wavFiles/tuba-sus-g2-PB-loop.wav")
    # print(fSeries)

    # song, fs = synthesizeTest()

    # rate, data = wavfile.read("wavfiles/TENORSA.wav")
    # data = data * 1.0 / (abs(data).max())
    # if (data.ndim > 1):
    #     data = data.sum(axis=1) / 2
    #     period, rate = extractPeriod(data, rate)
    # else:
    #     period, rate = extractPeriod(data, rate)
    #
    # anbn = fourierSeries(period, 9)
    #
    # song = reconstruct(len(period), anbn)

    # song = ((song + song.min()) * (2 ** 15) / song.ptp()).astype(np.int16)
    # song = song * 1.0 / (abs(song).max())
    # song = song.astype(np.float32)
    # print(song)
    # wavfile.write("wavFiles/test_new0.wav", fs, song)

