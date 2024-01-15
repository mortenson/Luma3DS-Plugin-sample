#pragma once

// Those macros are from Luma3DS - https://github.com/AuroraWright/Luma3DS
// {
#define PA_PTR(addr)            (void *)((u32)(addr) | 1 << 31)

#ifndef PA_FROM_VA_PTR
#define PA_FROM_VA_PTR(addr)    PA_PTR(svcConvertVAToPA((const void *)(addr), false))
#endif

#define REG32(addr)             (*(vu32 *)(PA_PTR(addr)))

#define HID_PAD           (REG32(0x10146000) ^ 0xFFF)

// }

#define STACK_SIZE        (0x1000)
