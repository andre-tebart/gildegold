/* 0x51d2d0, after a load: builds the scene (0x44be60 with the loader 0x537b60), the scene tree (0x45eeb0, size 0x100, 0x80 when the scene kind is 0x40), sets the detail bits (+0x71c, mask 0x1c, from 0x61adf8) and registers the two scene callbacks 0x51d4f0 (0x1c0) / 0x51d530 (0xc0) with 0x438e60. Returns 1 @deep B3 code hunks 52: same operations, VC6 scheduling/register shape - the scene size (0x100, 0x80 when the kind is 0x40) is materialised in eax and pushed once where the original duplicates push 2 per branch and pushes the constant at each site (0x51d323/0x51d32c); ours saves esi (the 'saved' local 0x633444) before the 0x45eeb0 call (push esi at 0x51d32d) and cleans 0x14 bytes later, the original pushes esi after the call (0x51d341) with the 45eeb0 args cleaned at 0x51d339 and mov ds:0xfd7c50,eax before the push; the +0x1c70 byte xor is only a register choice (eax/cl vs ecx/al). Read the whole function; tried the size select as an if/else and as an inline ternary, VC6 emits the same. Q1/Q2/Q3/Q5 open (0x537b60/0x51d4f0/0x51d530 function pointers, 0x71c field) */


int unk_Unk51d2d0(int f)
{
    int saved, size;
    unsigned char *b;

    d3if_SelectScene(1, 0);
    d3if_ClearScene(0);
    d3if_SelectScene(0, 0);
    d3_LoadScene(0, f, (void *)(unsigned)&main_Unk537b60);
    d3if_SelectScene(1, 0);
    d3if_SelectScene(0, 0);
    *(int *)ai_Unkfd7c50 = d3sm_Create(ai_Unk633738,
                                (*ai_Unk633738 == 0x80 || *ai_Unk633738 != 0x40) ? 0x100 : 0x80, 2);
    saved = d3_CurrentScene;
    d3if_SelectScene(0, 0);
    if (ai_Unk633738 != 0) {
        b = (unsigned char *)ai_Unk633738 + 0x71c * 4;
        *b = ((init_Detail << 2 ^ *b) & 0x1c) ^ *b;
        d3fl_SetDetail(0x80);
    }
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0x1c0, (void *)(unsigned)&ls_HookBuildingCB);
    d3_ForEachObject2(ai_Unk5fb5b0, 0, 0xc0, (void *)(unsigned)&ls_ClearObjectFlag);
    d3if_SelectScene(saved, 0);
    return 1;
}
