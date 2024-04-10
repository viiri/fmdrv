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

#ifndef CMF_H
#define CMF_H

#define READ_16LE(p) ((uint16_t)(p)[0] | ((uint16_t)(p)[1] << 8))

uint16_t sbfm_version(void);
uint8_t *sbfm_status_addx(uint8_t *new);
void sbfm_instrument(uint8_t *inst_table, int num_inst);
void sbfm_song_speed(uint16_t fdiv);
void sbfm_play_music(uint8_t *cmf_music_blk);
void sbfm_pause_music(void);
void sbfm_resume_music(void);
int sbfm_init(size_t srate);
void sbfm_tick(void);
int16_t sbfm_render(void);
void sbfm_reset(void);

#endif