function song = playBachTrumpet()
%{
    PLAYSONG: Produce a sinusoidal waveform containing the combination of the different notes in theVoices
    Input Args:
        theVoices: structure contains noteNumbers, durations, and startpulses vectors for multiple voices of a song.
    Output:
        song: vector that represents discrete-time version of a musical waveform
    Usage:
        song = playSong()
%}
    load bach_fugue.mat
    fs = 44100;
%     beats_per_minute = 120;
%     beats_per_second = beats_per_minute / 60;
%     seconds_per_beat = 1 / beats_per_second;
    spp = .15;%seconds per pulse, theVoices is measured in pulses with 4 pulses per beat
    max = 0;
    Ak = [0.1155 .3417 0.1789 0.1232 0.0678 0.0473 0.0260 0.0045 0.0020];
    phik = [-2.1299 1.6727 -2.5454 .6607 -2.0390 2.1597 -1.0467 1.8581 -2.3925];
    for ii = 1:length(theVoices)
        for jj = 1:length(theVoices(ii).startPulses)
            check = theVoices(ii).startPulses(jj) + theVoices(ii).durations(jj);
            if check > max
                max = check;
            
            end
        end
    end
    song = zeros(1,spp*fs*max);%Create a vector of zeros with length equal to the total number of samples in the entire song
    
    %Then add in the notes
    for ii = 1:length(theVoices)
        for jj = 1:length(theVoices(ii).noteNumbers)
            keynum = theVoices(ii).noteNumbers(jj);
            freq = 440*(2^((keynum-49)/12));
            dur = spp*theVoices(ii).durations(jj);
            tt = 0:(1/fs):dur-1/fs;
            %E = hanning(length(tt),'periodic');
            note = Ak(1)*cos(cos(freq*1*tt-phik(1)));%Create sinusoid of correct length to represent a single note
            for kk = 2:9
               note = note + Ak(kk)*cos(cos(freq*kk*tt-phik(kk))); 
            end
            locstart = spp*fs*theVoices(ii).startPulses(jj);%Index of where note starts
            locend = locstart + length(tt)-1;% index of where note ends
            
            song(locstart:locend) = song(locstart:locend) + note;
        end
    end
    
    %soundsc() or audiowrite()
    %soundsc(song,fs);
    
    %Question: Suppose the maximum frequency in the Bach Fugue is 1200 Hz. What is the minimum 
    %samplingfrequency needed to synthesize, without aliasing, a trumpet sound containing nine harmonics?

    %Answer: 2400 Hz
end

