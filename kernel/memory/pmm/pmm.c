#include "pmm.h"

uint8_t *frame_bitmap;
uint32_t total_frames;
uint32_t free_frames;
uint32_t bitmap_size;

void pmm_init() {
    total_frames = total_memory / FRAME_SIZE;
    free_frames = total_frames;
    bitmap_size = (total_frames + FRAMES_PER_BYTE - 1) / FRAMES_PER_BYTE;
    frame_bitmap = (uint8_t*)((kernel_end + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1));

    for (uint32_t i = 0; i < bitmap_size; i++)
        frame_bitmap[i] = 0;
    
    uint32_t bitmap_end = (uintptr_t)frame_bitmap + bitmap_size;
    uint32_t used_frames = (bitmap_end + FRAME_SIZE - 1) / FRAME_SIZE;

    for (uint32_t i = 0; i < used_frames; i++) {
        uint32_t byte_index = i / FRAMES_PER_BYTE;
        uint32_t bit_index = i % FRAMES_PER_BYTE;

        frame_bitmap[byte_index] |= (1 << bit_index);
        free_frames--;
    }

    uint32_t next_available_address = (bitmap_end + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1);

    printf("pmm... [OK]\n");

    if (debug)
        printf("    frame_bitmap address: 0x%p\n    bitmap size: %u bytes\n    start of free memory: 0x%x\n", (void*)frame_bitmap, bitmap_size, next_available_address);
}

uint32_t pmm_alloc_frame() {
    if (free_frames == 0) {
        kpanic(0);

        return 0;
    }

    for (uint32_t i = 0; i < bitmap_size; i++) {
        if (frame_bitmap[i] != 0xff) {
            for (uint32_t j = 0; j < FRAMES_PER_BYTE; j++) {
                if (!(frame_bitmap[i] & (1 << j))) {
                    frame_bitmap[i] |= (1 << j);
                    free_frames--;

                    uint32_t frame_index = i * FRAMES_PER_BYTE + j;

                    return frame_index * FRAME_SIZE;
                }
            }
        }
    }

    return 0;
}

void pmm_free_frame(uint32_t frame_addr) {
    uint32_t frame_index = frame_addr / FRAME_SIZE;
    uint32_t byte_index = frame_index / FRAMES_PER_BYTE;
    uint32_t bit_index = frame_index % FRAMES_PER_BYTE;

    if (!(frame_bitmap[byte_index] & (1 << bit_index)))
        return;
    
    frame_bitmap[byte_index] &= ~(1 << bit_index);
    free_frames++;
}