function song = playBach()
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
    %load tempSong.mat
    fs = 8000;
%     beats_per_minute = 120;
%     beats_per_second = beats_per_minute / 60;
%     seconds_per_beat = 1 / beats_per_second;
    spp = .15;%seconds per pulse, theVoices is measured in pulses with 4 pulses per beat
    max = 0;
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
            E = hanning(length(tt),'periodic');
            note = E'.*real(exp(j*2*pi*freq*tt));%Create sinusoid of correct length to represent a single note
            locstart = spp*fs*theVoices(ii).startPulses(jj);%Index of where note starts
            locend = locstart + length(tt)-1;% index of where note ends
            
            song(locstart:locend) = song(locstart:locend) + note;
        end
    end
    
    %soundsc() or audiowrite()
    %soundsc(song,fs);
    
end

