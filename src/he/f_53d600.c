/* 0x53d600, sets the alloc / run handlers of the event type `type` (0 .. 0xa6; 1 = type out of range) */

int he_Unk53d600(int type, void *alloc, void *run)
{
    if (type >= 0 && type < 0xa7) {
        U_I(((unsigned)&he_QuickList + 0x400) + type * 4) = (int)alloc;
        U_I((unsigned)&he_RunHandlers + type * 4) = (int)run;
        return 0;
    }
    return 1;
}
