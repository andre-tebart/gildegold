/* 0x4b4e30, mission condition: 0x4f9e00 at most 0.1 and its values 0x0c, 0x04, 0x08 at least 0.3 */

int ai_Unk4b4e30(AICHAR *c, G13TYPE *t)
{
    G13ECON e;

    if (unk_Unk4f9e00(&e) > 0.1)
        return 0;
    if (e.unk0c < 0.3f)
        return 0;
    if (e.unk04 < 0.3f)
        return 0;
    if (e.unk08 < 0.3f)
        return 0;
    return 1;
}
