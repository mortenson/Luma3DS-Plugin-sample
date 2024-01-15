#include <3ds.h>
#include <string.h>
#include <stdio.h>
#include "plgldr.h"
#include "csvc.h"
#include "common.h"

static PluginMenu   menu;
static Handle       thread;
static Handle       onProcessExitEvent, resumeExitEvent;
static u8           stack[STACK_SIZE] CTR_ALIGN(8);

// Plugin main thread entrypoint
void    ThreadMain(void *arg)
{
    // Plugin main loop
    while (1)
    {
        if (HID_PAD & BUTTON_SELECT)
            PLGLDR__DisplayMessage(">", "わりやまはなたさかあ\nをりゆみひにちしさい\nんるよむふめつすくう\n゛れ！めへねてせけえ\n゜ろ？もほのとぞこお");
        svcSleepThread(50000000);
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

    // Result rc = romfsInit();
    // if (rc)
    //     printf("romfsInit: %08lX\n", rc);
    // else
    // {
    //     FILE* f = fopen(path, "romfs:/jishou/JMdict_smol.txt");
    //     if (f)
    //     {
    //         char mystring[100];
    //         while (fgets(mystring, sizeof(mystring), f))
    //         {
    //             int a = strlen(mystring);
    //             if (mystring[a-1] == '\n')
    //             {
    //             mystring[a-1] = 0;
    //             if (mystring[a-2] == '\r')
    //             mystring[a-2] = 0;
    //             }
    //             puts(mystring);
    //             }
    //             printf(">>EOF<<\n");
    //             fclose(f);
    //         }
    // }
    // romfsExit();

    // Create the plugin's main thread
    svcCreateThread(&thread, ThreadMain, 0, (u32 *)(stack + STACK_SIZE), 0x1A, 0);

    svcInvalidateEntireInstructionCache();
}
