#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
SDL_AudioDeviceID audioDevice;  // Declare the audio device globally

// Initialize SDL audio device
void initAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_AudioSpec desiredSpec;
    SDL_AudioSpec obtainedSpec;

    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 4096;  // Buffer size
    desiredSpec.callback = nullptr;  // We're not using a callback for this simple case

    // Open the audio device
    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, &obtainedSpec, 0);
    if (audioDevice == 0) {
        std::cerr << "Error opening audio device: " << SDL_GetError() << std::endl;
    }
}

// Stop the audio device (clean up when done)
void closeAudio() {
    SDL_CloseAudioDevice(audioDevice);
}

// Function to play a tone
void playTone(int frequency, int duration) {
    const int sampleRate = 44100;
    const int amplitude = 18000;
    const int sampleCount = duration * sampleRate / 1000;
    Sint16* samples = new Sint16[sampleCount];

    // Generate sine wave for the tone
    for (int i = 0; i < sampleCount; ++i) {
        samples[i] = amplitude * sin(2 * M_PI * frequency * i / sampleRate);
    }

    // Queue the audio samples to the audio device
    
    SDL_QueueAudio(audioDevice, samples, sampleCount * sizeof(Sint16));
    SDL_PauseAudioDevice(audioDevice, 0);  // Start playing the queued audio

    SDL_Delay(duration);  // Wait for the sound to finish (not blocking game loop)

    delete[] samples;
}


// Example sound effects
void playEatSound() {
    playTone(523, 25);  // 523Hz for 200ms (when fruit is eaten)
}

void playGameOverSound() {
    playTone(392, 500);  // 392Hz for 500ms (when game is over)
}