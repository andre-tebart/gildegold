/* 0x57bf00, the tutorial results window (1629 B, ROUGH SKELETON): a 0x55c-byte local frame; 0x4fb330 (0, 0x52);
   the three player records collected (0x4ec2d0 (i) when non-null, then 0x4ec320 (p) - the names -, i = 0 .. 2);
   when no player was found the byte 0x6f1bc5 seeds a fallback list (the [esp+0x168] buffer). Then the window
   0x4741f0 and its controls / texts (0x475f70, 0x473650 x8, 0x4760b0 x5, 0x5176d0 x9, 0x52eaa0 x4, 0x521410,
   0x52e350, 0x52ea30), 0x57c560 (the results list), the release 0x4758f0, 0x4731d0 x2, 0x476f80, 0x475e40,
   0x475600 and the pump 0x5309a0 (0x86, ...); the rest of the body is not written yet (see the disassembly
   build/dis57bf00.txt) */

struct AHMSTEP * ahm_WalkStep();
int ahm_StepValue();
int ai_Unk57c560();

int ai_Unk57bf00(void *rec)
{
    char *per[3];
    char *nm[3];
    char *v;
    int n;
    int i;

    v = (char *)unk_Unk4fb330(0, 0x52);
    n = 0;
    for (i = 0; i < 3; i++) {
        per[n] = (char *)ahm_WalkStep(i);
        if (*(int *)per[n]) {
            nm[n] = (char *)ahm_StepValue(per[n]);
            n++;
        }
    }
    /* ROUGH: the n == 0 fallback (the byte 0x6f1bc5, the local buffer at +0x168), the window and its controls,
       the results built by 0x57c560, the pump 0x5309a0 (0x86) and the tail are not written yet */
    return (int)v;
}
