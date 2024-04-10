/*
 * fmdrv - very accurate C port of SBFMDRV CMF replayer.
 *
 * Copyright 2024 Sergei "x0r" Kolzun
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <SDL.h>

#include "fmdrv.h"

#define SAMPLING_FREQ  48000
#define BUFFER_SAMPLES 16384

static void fill_audio(void *udata, Uint8 *stream, int len)
{
	int i;

	for(i = 0; i < len / 2; ++i)
		((Uint16 *)(stream))[i] = sbfm_render();
}

static int sdl_init()
{
	SDL_AudioSpec audiospec;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "sdl: can't initialize: %s\n", SDL_GetError());
		return -1;
	}

	audiospec.freq = SAMPLING_FREQ;
	audiospec.format = AUDIO_S16SYS;
	audiospec.channels = 1;
	audiospec.samples = BUFFER_SAMPLES;
	audiospec.callback = fill_audio;
	audiospec.userdata = NULL;

	if (SDL_OpenAudio(&audiospec, NULL) < 0) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}

	return 0;
}

uint8_t *load_cmf(const char *filename)
{
	FILE *fp;
	long size;
	uint8_t *data;

	fp = fopen(filename, "rb");

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	data = (uint8_t *)malloc(size);
	if(data == NULL)
		goto err;

	if(fread(data, 1, size, fp) != size) {
		free(data);
		data = NULL;
	}

err:
	fclose(fp);

	return data;
}

int main(int argc, char *argv[])
{
	uint8_t status;
	uint8_t *data;

	if(argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		exit(-1);
	}

	data = load_cmf(argv[1]);
	if(data == NULL) {
		printf("Can't load: %s <filename>\n", argv[0]);
		exit(-1);
	}

	sbfm_init(SAMPLING_FREQ);
	sbfm_reset();
	sbfm_status_addx(&status);
	sbfm_instrument(&data[READ_16LE(&data[0x06])], READ_16LE(&data[0x24]));
	sbfm_song_speed(0x1234dc / READ_16LE(&data[0x0c]));
	sbfm_play_music(&data[READ_16LE(&data[0x08])]);

	if(sdl_init() < 0)
	{
		fprintf(stderr, "%s: can't initialize sound\n", argv[0]);
		exit(-2);
	}

	SDL_PauseAudio(0);

	while(status)
		SDL_Delay(10);

	SDL_CloseAudio();
	free(data);

	return 0;
}
