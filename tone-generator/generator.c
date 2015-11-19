#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <math.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

/* These are in charge of maintaining our sine function */
float sinPos;
float sinStep;

/* These are the audio card settings */
#define FREQ 44100
#define SAMPLES 8192 

/* This is basically an arbitrary number */
#define VOLUME 127.0

void populate(void* data, Uint8 *stream, int len) {
	int i=0;
	for (i=0; i<len; i++) {
		/* Just fill the stream with sine! */
		stream[i] = (Uint8) (VOLUME * sinf(sinPos))+127;
		sinPos += sinStep;
	}
}

void usage() {
	puts ("Usage: generator freq [duration]\nWhere:\n\tfreq is the frequency of the tone, given in Hz.\n\tduration is optional. It defines the number of seconds the tone should play for.\n\t\tIf not given, it plays for 1 second.");
}

int main(int argc, char* argv[]) {
	/* This will hold our data */
	SDL_AudioSpec spec;
	/* This will hold the requested frequency */
	long reqFreq = 440;
	/* This is the duration to hold the note for */
	int duration = 1;

	/* Process Command Line Arguments */
	if (argc <= 1) {
		/* Nothing Given, output usage */
		usage();
		exit(EXIT_FAILURE);
	} else if (argc >= 2) {
		/* Has frequency */
		reqFreq = strtol(argv[1], NULL, 10);
		if (errno == EINVAL) {
			fprintf (stderr, "Frequency '%s' is invalid\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		if (argc >= 3) {
			/* Has duration */
			duration = (int) strtol(argv[2], NULL, 10);
			if (errno == EINVAL) {
				fprintf (stderr,"Duration '%s' is invalid\n", argv[2]);
				exit(EXIT_FAILURE);
			}
		}

		if (argc >=4) {
			/* Who knows what's here */
			puts ("Warning: Arguments found past frequency and duration, disregarding them\n");
		}
	} 

	/* Set up the requested settings */
	spec.freq = FREQ;
	spec.format = AUDIO_U8;
	spec.channels = 1;
	spec.samples = SAMPLES;
	spec.callback = (*populate);
	spec.userdata = NULL;

	/* Open the audio channel */
	if (SDL_OpenAudio(&spec, NULL) < 0) { 
		/* FAIL! */
		fprintf(stderr, "Failed to open audio: %s \n", SDL_GetError());
		exit(1); 
	} 

	/* Initialize the position of our sine wave */
	sinPos = 0;
	/* Calculate the step of our sin wave */
	sinStep = 2 * M_PI * reqFreq / FREQ;

	/* Now, run this thing */
	SDL_PauseAudio(0); 
	/* Delay for the requested number of seconds */
	sleep(duration);
	/* Then turn it off again */
	SDL_PauseAudio(1);

	/* Close audio channel */
	SDL_CloseAudio();

	return 0;
}
