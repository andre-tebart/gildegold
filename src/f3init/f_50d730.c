/* 0x50d730, the callback for the light objects (type 6) of a loaded scene: a light without a third value gets one (the sum of the two others times 0.05) and its colours are scaled (0.8 / 0.5) @deep far, 61 vs 59 instructions, 22 differing; every hunk was read against the original 0x50d730..0x50d7e3 and none is a meaning error: the only differences are the addressing / scheduling forms - q = l + i is built with add vs lea and placed after the fcomp, and the original reloads *(o+0x1e8) between the three float stores (our source reloads it too, VC6 keeps the first load). The o+0x64 block is identical instruction for instruction. spell.py: no hunk matched the menu (50d744 / 50d7ee have no rule), so nothing in the last-bytes pass is left to try. Q4 is all that stays: 22 raw accesses - D3OBJECT +0x218 (typed in d3.h) and +0x5c / +0x60 / +0x64 (d3.h D3OBJECT.unk05c, a D3VECTOR whose three floats this light code reads as colour components), +0x94 (unk090.z) and +0x1e8 (untyped pad1d8, a pointer to a light record whose floats at 0x18 / 0x1c / 0x20 / 0x24 have no type anywhere). Naming them is a shared-header decision (d3.h D3OBJECT / a light record type) of the same kind as init_HookObject's +0x200, and the roles are not evidenced beyond this one callback. Q1 init_AdjustSceneLights (0x50d6d0's comment: it adjusts the lights of a loaded scene), Q2/Q3/Q6 pass, Q5 ROUGH removed. */


void init_AdjustSceneLights(int o)
{
    int i, q;
    int *l;

    if (*(int *)(o + 0x218) == 6) {
        i = 0;
        do {
            q = *(int *)(o + 0x1e8) + i;
            if (*(float *)(q + 0x20) == 0.0f) {
                *(float *)(q + 0x20) = (*(float *)(q + 0x1c) + *(float *)(q + 0x18)) * 0.05f;
                *(float *)(*(int *)(o + 0x1e8) + 0x1c + i) = *(float *)(*(int *)(o + 0x1e8) + 0x1c + i) * 0.8f;
                *(float *)(*(int *)(o + 0x1e8) + 0x24 + i) = *(float *)(*(int *)(o + 0x1e8) + 0x24 + i) * 0.5f;
            }
            i += 0x38;
        } while (i < 0x188);
        if (*(float *)(o + 100) == 0.0f) {
            *(float *)(o + 100) = (*(float *)(o + 0x5c) + *(float *)(o + 0x60)) * 0.05f;
            *(float *)(o + 0x60) = *(float *)(o + 0x60) * 0.8f;
            *(float *)(o + 0x94) = *(float *)(o + 0x94) * 0.5f;
        }
    }
}
