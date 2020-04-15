import numpy as np
from scipy.io import wavfile
import matplotlib.pyplot as plt
import requests
from io import BytesIO
import IPython.display
from IPython.display import Audio, HTML


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


def reconstruct(P, anbn, rate):
    """Sum up sines and cosines according to the coefficients to
    produce a reconstruction of the original waveform"""
    result = 0
    t = np.arange(P*1000)
    freq = 490

    for n, (a, b) in enumerate(anbn):
        if n == 0:
            a = a/2
        result = result + a*np.cos(2*np.pi*n*t*freq/rate) + b * np.sin(2*np.pi*n*t*freq/rate)

    freq = 330
    for n, (a, b) in enumerate(anbn):
        if n == 0:
            a = a/2
        result = result + a*np.cos(2*np.pi*n*t*freq/rate) + b * np.sin(2*np.pi*n*t*freq/rate)

    freq = 390
    for n, (a, b) in enumerate(anbn):
        if n == 0:
            a = a / 2
        result = result + a * np.cos(2 * np.pi * n * t * freq / rate) + b * np.sin(2 * np.pi * n * t * freq / rate)

    return result


def loadSound(url):
    R = requests.get(url)
    rate, data = wavfile.read(BytesIO(R.content))
    data = data * 1.0 / (abs(data).max())
    if len(data.shape) > 1:  # for stereo data, use only first channel
        data = data[:,0]
    return data, rate


# Utility function two display two audios side by side in the notebook
def audioSideBySide(name1, audio1, name2, audio2):
    text = '%s	%s %s	%s' % (name1, name2, audio1._repr_html_(), audio2._repr_html_())
    IPython.display.display(HTML(text))


def extractPeriod(data, rate, t_start, t_end):
    t = np.arange(0,len(data))/rate
    plt.plot(t, data)

    duration = t_end - t_start
    plt.xlabel('$t$'); plt.ylabel('$x(t)$');


    sample_start = int(t_start * rate)
    sample_end = int(t_end*rate)
    print(rate)
    period = data[sample_start:sample_end]
    audioSideBySide("Original", Audio(data=data,rate=rate),
                    "Extracted period", Audio(np.tile(period, int(1/duration)), rate=rate))
    return period, rate


def approximateUpToNthHarmonic(period, rate, N):
    t = np.arange(len(period)) / rate
    duration = t.max()
    F = fourierSeries(period, N)
    powers = np.sqrt(np.sum(F ** 2, axis=1))
    print(F)
    reconstructed = reconstruct(len(period), F, rate)

    plt.subplot(121)
    plt.plot(t * 1000, period, label='Original')
    # plt.plot(t * 1000, reconstructed, label='Reconstructed')
    # plt.subplot(122)
    plt.stem(powers)

    audioSideBySide("Original", Audio(data=np.tile(period, int(0.7 / duration)), rate=rate),
                    "Reconstructed up to %d harmonics" % N,
                    Audio(data=np.tile(reconstructed, int(0.7 / duration)), rate=rate))

    # plt.show()
    return reconstructed


def finadAndExtractPeriod(data, rate):
    maxA = max(data[len(data) // 2:])
    print(max(data), maxA)
    found = 0
    t_start = 0
    t_end = 0
    for i in range(len(data)):
        if data[i] >= .95 * maxA and found == 0:
            t_start = i
            found = 1
        elif data[i] >= .95 * maxA and found == 1:
            t_end = i
            found = 2

    print(t_start, t_end)
    sample_start = int(t_start * rate)
    sample_end = int(t_end * rate)
    # print(sample_start, sample_end)
    period = data[t_start:t_end]

    # print(len(data), len(period))

    return period, rate


if __name__ == '__main__':
    # the URLs of the wav files
    urls = ["http://cd.textfiles.com/sbsw/INSTRMNT/TENORSA.WAV"]
    sounds = []
    for url in urls:
        sound, rate = loadSound(url)
        sounds.append((sound, rate))

    for n, s in enumerate(sounds):
        IPython.display.display(HTML("Saxophone sound % d" % (n + 1) + Audio(data=s[0], rate=s[1])._repr_html_()))

    t = np.arange(len(sounds[0][0])) / sounds[0][1]
    plt.subplot(211)
    plt.plot(t, sounds[0][0])
    plt.subplot(212)
    plt.plot(t, sounds[0][0])
    # plt.show()

    periods = []
    rates = []
    # The manually found start and end time of one period of the signal
    periodBounds = [(0.8044, 0.8075)]
    for n, (S, P) in enumerate(zip(sounds, periodBounds)):
        plt.subplot(len(sounds), 1, n + 1)
        # period, rate = extractPeriod(S[0], S[1], P[0], P[1])
        period, rate = finadAndExtractPeriod(S[0], S[1])
        periods.append(period);
        rates.append(rate)

    waves = []
    for P, R in zip(periods, rates):
        waves.append(approximateUpToNthHarmonic(P, R, 10))

    # Audio(data = waves[0], rate = rate)
    # print(waves[0])
    # waves[0] = ((waves[0] + waves[0].min()) * (2 ** 15) / waves[0].ptp()).astype(np.int16)
    waves[0] = waves[0].astype(np.float32)
    print(waves[0])
    wavfile.write("wavFiles/test_fourier2.wav", rates[0], waves[0])