#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stdbool.h>
#include "../../kernel.h"
#include "../../util/print.h"
#include "../../util/panic.h"
#include "../mmap/mmap.h"

#define FRAME_SIZE 4096
#define FRAMES_PER_BYTE 8

extern uint8_t *frame_bitmap;
extern uint32_t total_frames;
extern uint32_t free_frames;
extern uint32_t bitmap_size;

void pmm_init();

uint32_t pmm_alloc_frame();

void pmm_free_frame(uint32_t frame_addr);

#endif