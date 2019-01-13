#include <stdio.h>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>
#include <math.h>
#include "wav_importer.h"

int main(int argc, char** argv) {
    ALCdevice* device;
    ALCcontext* context;
    unsigned int source;
    unsigned int audioBuffer;

    // loading a WAV file
    FILE * soundFile = fopen("drum.wav", "rb");
    fseek(soundFile, 0, SEEK_END);
    long size = ftell(soundFile);
    rewind(soundFile);
    char* fileData = new char[size];
    fread(fileData, 1, size, soundFile);
    WavFile wf = loadWavFileData(fileData);
    printWaveFile(wf);

    device = alcOpenDevice(0);
    context = alcCreateContext(device, 0);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &audioBuffer);

    char sound[44100];
    for (int i = 0; i < 44100; i++) {
        sound[i] = (sin(i) + 1) / 2 * 255;
    }

    alBufferData(audioBuffer, AL_FORMAT_STEREO16, wf.data, wf.subChunk2Size, wf.sampleRate);
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, audioBuffer);
    alSourcei(source, AL_LOOPING, true);

    alSourcePlay(source);

    getchar();

    printf("Hello world.\n");

    return 0;
}