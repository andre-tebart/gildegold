/* 0x56ddc0, true when the building b has a task of the message kinds 0x24b / 0x256 / 0x266 in progress (0x4ffe90 (b's +0x5d, 2, 6, 0, kind), its +0x1c above 0) */

int he_Unk56ddc0(int b, int kind)
{
    int h;

    if (kind == 0x24b || kind == 0x256 || kind == 0x266) {
        h = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, kind);
        if (h != 0 && *(int *)(h + 0x1c) > 0)
            return 1;
    }
    return 0;
}
