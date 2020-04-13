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

