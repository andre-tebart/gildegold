/* 0x53d4b0, the init of the event module: registers the alloc / run handlers of the types 0 and 1, runs the init of the other event modules (0x553060 .. 0x573260; non 0 = failed) and clears the tables */

int he_Unk53d4b0(void)
{
    if (he_Unk53d600(0, (void *)he_Unk53d430, (void *)he_Unk53d470) != 0)
        return 1;
    if (he_Unk53d600(1, (void *)d3io_NoopCallback, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_Unk553060() != 0)
        return 1;
    if (he_Unk568310() != 0)
        return 1;
    if (ai_Unk56d050() != 0)
        return 1;
    if (he_Unk56f980() != 0)
        return 1;
    if (ai_Unk56d220() != 0)
        return 1;
    if (ai_Unk56e400() != 0)
        return 1;
    if (ai_Unk5733c0() != 0)
        return 1;
    if (ai_Unk573260() != 0)
        return 1;
    memset((void *)((unsigned)&he_QuickList + 0x6a0), 0, 0x2900 * 4);
    memset((void *)((unsigned)&he_RunHandlers + 0x6a0), 0, 0x1400 * 4);
    memset((void *)((unsigned)&he_RunHandlers + 0x2a0), 0, 0x100 * 4);
    return 0;
}
