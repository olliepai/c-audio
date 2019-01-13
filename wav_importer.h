#pragma once
#include <stdio.h>

#define PAD 1095761920
#define DATA 'atad'

struct WavFile {
    unsigned int chunkID;
    unsigned int chunkSize;
    unsigned int format;
    unsigned int subChunk1ID;
    unsigned int subChunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    unsigned int subChunk2ID;
    unsigned int subChunk2Size;
    char* data;
};

void printWaveFile(WavFile file){
    printf("Chunk ID: %c%c%c%c\n", file.chunkID, file.chunkID >> 8, file.chunkID >> 16, file.chunkID >> 24);
    printf("Chunk Size: %u\n", file.chunkSize);
    printf("Format: %c%c%c%c\n", file.format, file.format >> 8, file.format >> 16, file.format >> 24);
    printf("Sub Chunk 1 ID:  %c%c%c%c\n", file.subChunk1ID, file.subChunk1ID >> 8, file.subChunk1ID >> 16, file.subChunk1ID >> 24);
    printf("Sub Chunk 1 Size: %u\n", file.subChunk1Size);
    printf("Audio Format: %u\n", file.audioFormat);
    printf("Num Channels: %u\n", file.numChannels);
    printf("Sample Rate: %u\n", file.sampleRate);
    printf("Byte Rate: %u\n", file.byteRate);
    printf("Block Align: %u\n", file.blockAlign);
    printf("Bits Per Sample: %u\n", file.bitsPerSample);
    printf("Sub Chunk 2 ID:  %c%c%c%c\n", file.subChunk2ID, file.subChunk2ID >> 8, file.subChunk2ID >> 16, file.subChunk2ID >> 24);
    printf("Sub Chunk 2 Size: %u\n", file.subChunk2Size);

    printf("Data: \n");
    // for(int i = 0; i < file.subChunk2Size; i++){
    //     printf("%i", file.data[i]);
    // }
}

WavFile loadWavFileData(char* data){
    char* fileptr = data;
    WavFile wf;
    wf.chunkID = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.chunkSize = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.format = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.subChunk1ID = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.subChunk1Size = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.audioFormat = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.numChannels = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.sampleRate = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.byteRate = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.blockAlign = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.bitsPerSample = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.subChunk2ID = *(unsigned int*)fileptr;
    fileptr += 4;

    while(wf.subChunk2ID != DATA){
        if(wf.subChunk2ID == PAD){
           fileptr += 4;
        }
        unsigned int v = *(unsigned int*)fileptr;
        fileptr += 4;
        fileptr += v;
        wf.subChunk2ID = *(unsigned int*)fileptr;
        fileptr += 4;
    }

    wf.subChunk2Size = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.data = new char[wf.subChunk2Size];
    if(wf.bitsPerSample == 24){
        char* iptr = fileptr;
        unsigned int ctr = 0;
        for(int i = 0; i < wf.subChunk2Size; i += 3){
            int num = *(int*)iptr / 256;
            iptr += 3;
            wf.data[ctr++] = num << 8;
            wf.data[ctr++] = num >> 8;
        }
    }else{
        
        for(int i = 0; i < wf.subChunk2Size; i++){
            wf.data[i] = *fileptr++;
        }
    }

    return wf;
}

void convertStereoToMonoWav(WavFile* wf){
    wf->numChannels = 1;
    unsigned int newSize = 0;
    if(wf->bitsPerSample == 24){
        newSize = (((float)wf->subChunk2Size * 2.0) / 3.0 )/ 2;
    }else{
        newSize = wf->subChunk2Size / 2;
    }
    
    char* d = new char[newSize];
    short* p = (short*)wf->data;
    short* nd = (short*)d;
    for(int i = 0; i < newSize / 2; i++){
        short newValue = *p++ / 2;
        newValue += *p++ / 2;
        *nd = newValue;
        nd++;
    }
    wf->subChunk2Size = newSize;
    delete[] wf->data;
    wf->data = d;
}

void freeWavFileData(WavFile wf){
    if(wf.data){
        delete[] wf.data;
        wf.data = 0;
    }
}