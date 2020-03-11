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


def play():
    voices = [{"startPulses": [1, 20, 30], "durations": [20, 10, 5], "noteNumbers": [40, 42, 44]}]

    # print np.asarray(voices[0]["startPulses"])
    fs = 8000

    spp = .15
    maxl = 0

    for ii in range(len(voices)):
        for jj in range(len(voices[ii]["startPulses"])):
            temp = voices[ii]["startPulses"][jj] + voices[ii]["durations"][jj]
            if temp > maxl:
                maxl = temp

    song = np.zeros(int(spp*fs*maxl))

    for ii in range(len(voices)):
        for jj in range(len(voices[ii]["noteNumbers"])):
            keynum = voices[ii]["noteNumbers"][jj]
            freq = 440 * (2**((keynum - 49) / 12.))
            dur = spp*voices[ii]["durations"][jj]
            tt = np.arange(0., dur, (1. / fs), dtype=float)
            E = np.hanning(len(tt))
            z = np.exp(1j*2*math.pi*freq*tt)
            note = np.asarray(E.transpose()*z.real)  # Create sinusoid of correct length to represent a single note
            locstart = int(spp * fs * voices[ii]["startPulses"][jj])  # Index of where note starts
            locend = int(locstart + len(tt))  # index of where note ends

            song[locstart:locend] = song[locstart:locend] + note

    return song


if __name__ == '__main__':
    s = play()
    s = ((s + s.min()) * (2 ** 15) / s.ptp()).astype(np.int16)

    wavfile.write('wavFiles/test.wav', 8000, s)
    # AudioSegment.from_wav("wavFiles/test.wav").export("mp3Files/test.mp3", format="mp3")


