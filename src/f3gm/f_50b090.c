/* 0x50b090, the class number (out) of the building kind k: 1 for 0xf, 2 .. 5 for 0x17 .. 0x1a (returns 1), 6 / 7 / 7 for 7 / 4 / 0x10 (returns 0) */

int gm_ResearchClass(int k, int *out)
{
    int r;

    r = 1;
    switch (ai_BTypes[k * 0x24d]) {
    case 4:
        *out = 7;
        return 0;
    case 7:
        *out = 6;
        return 0;
    case 0xf:
        *out = 1;
        return r;
    case 0x10:
        *out = 7;
        break;
    case 0x17:
        *out = 2;
        return r;
    case 0x18:
        *out = 3;
        return r;
    case 0x19:
        *out = 4;
        return r;
    case 0x1a:
        *out = 5;
        return r;
    }
    return 0;
}
