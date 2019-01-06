#include <stdio.h>
#include <OpenAl/al.h>
#include <OpenAl/alc.h>
#include <math.h>

int main(int argc, char** argv) {
    ALCdevice* device;
    ALCcontext* context;
    unsigned int source;
    unsigned int audioBuffer;

    device = alcOpenDevice(0);
    context = alcCreateContext(device, 0);
    alcMakeContextCurrent(context);
    alGenBuffers(1, &audioBuffer);

    char sound[44100];
    for (int i = 0; i < 44100; i++) {
        sound[i] = (sin(i) + 1) / 2 * 255;
    }

    alBufferData(audioBuffer, AL_FORMAT_MONO8, sound, sizeof(char) * 44100, 20000);
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, audioBuffer);
    alSourcei(source, AL_LOOPING, true);

    alSourcePlay(source);

    getchar();

    printf("Hello world.\n");

    return 0;
}