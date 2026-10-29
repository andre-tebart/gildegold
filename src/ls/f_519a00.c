/* 0x519a00, before a save: the objects of the buildings get their building back (+0x200 of the building's scene object, +0x61), and the references of the 10-byte table 0xf31166 (kind byte at -5: 1 character, 2 building, 3 object, 9 ..) are turned into plain ids (the id inside the record: +4 / +1 / +2 / +0) @deep B: 50 code hunks, read in asmdiff: they are register / scheduling shapes with no meaning difference - the first loop swaps the index and base registers (ours [edx+eax*1] vs the original [eax+edx*1], k in edx vs eax), the second loop's comparison polarity (ours cmp/jge, the original cmp/jl) and the *p reload: the original reloads [eax] after the store in the kind==3 / kind==9 branches, ours forwards the register (edx) - the kinds, offsets (+4/+2/+1/+0) and bounds (0xa900 / 0xf81166) match the original at 0x519a00 exactly. Shape is match-deep's. Q2/Q7: the callees d3if_SelectScene, ai_Unkf31166 and the referenced globals are unnamed; Q6 is described. */


void unk_Unk519a00(void)
{
    int saved = d3_CurrentScene;
    int k;
    int *p;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        if (ai_Blds[k] != 0 && *(int *)(ai_Blds + k + 0x61) != 0)
            *(int *)(*(int *)(ai_Blds + k + 0x61) + 0x200) = *(int *)(ai_Blds + k + 1);
    }
    d3if_SelectScene(saved, 1);
    for (p = ls_RefTargets; (int)p < (int)ls_RefTargets + 0x50000; p = (int *)((char *)p + 10)) {
        if (*p != 0) {
            char kind = *((char *)p - 5);

            if (kind == 1)
                *p = *(int *)(*p + 4);
            if (kind == 3)
                *p = *(int *)(*p + 2);
            if (kind == 2)
                *p = *(int *)(*p + 1);
            if (kind == 9)
                *p = *(int *)*p;
        }
    }
}
