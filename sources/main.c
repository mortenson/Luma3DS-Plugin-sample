#include <3ds.h>
#include <string.h>
#include <stdio.h>
#include "plgldr.h"
#include "csvc.h"
#include "common.h"
#include "draw.h"

static Handle       thread;
static u8           stack[STACK_SIZE] __attribute__((aligned(8)));

// Plugin main thread entrypoint
void    ThreadMain(void *arg)
{
    // Draw_Init();
    // Draw_Lock();
    // Draw_AllocateFramebufferCache(FB_BOTTOM_SIZE);
    // Draw_SetupFramebuffer();
    // Draw_ClearFramebuffer();
    // Draw_FlushFramebuffer();
    // Draw_Unlock();

    // svcSleepThread(5000000000);

    // Plugin main loop
    while (1)
    {
        // if (HID_PAD & BUTTON_SELECT) {
        // Draw_Lock();
        u16 *const fb = (u16 *)FB_BOTTOM_VRAM_ADDR;
        Draw_DrawString(0, 0, COLOR_WHITE, "Hello world you dumb pieces of shit");
        // Draw_Unlock();
        // }
        // PLGLDR__DisplayMessage("fuck", "さ");
        // svcSleepThread(5000000);
    }

exit:
    // This is executed when the game is about to exit
    // useful to save config, properly deinit stuff etc
    plgLdrExit();
    srvExit();

    svcExitThread();
}

extern char* fake_heap_start;
extern char* fake_heap_end;
extern u32 __ctru_heap;
extern u32 __ctru_linear_heap;

u32 __ctru_heap_size        = 0;
u32 __ctru_linear_heap_size = 0;

void    __system_allocateHeaps(PluginHeader *header)
{
    __ctru_heap_size = header->heapSize;
    __ctru_heap = header->heapVA;

    // Set up newlib heap
    fake_heap_start = (char *)__ctru_heap;
    fake_heap_end = fake_heap_start + __ctru_heap_size;
}

// Entrypoint, game will starts when you exit this function
void    main(void)
{
    PluginHeader *header = (PluginHeader *)0x07000000;

    // Init heap
    __system_allocateHeaps(header);

    // Init services
    srvInit();
    plgLdrInit();

    // Create the plugin's main thread
    svcCreateThread(&thread, ThreadMain, 0, (u32 *)(stack + STACK_SIZE), 0x1A, 0);

    svcInvalidateEntireInstructionCache();
}
