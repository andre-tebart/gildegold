/* 0x54fb50, a Ghidra miss: the init of the effect event: the object +0xac (0x4fea50) must have a scene record (+0x61) and the effect word +0xc2 a table record (0x5ba620) with a duration (+4): +0xb4 = now + that duration (0x4fe140), +0xb0 = 0, state 0; else the event is freed */

int ai_Unk5ba620(int param_1);
int d3t_CreateEmitter();
int mem_IsValid();
void par_Delete();
char *gm_FindBuildingById(int id);
int unk_Unk4fdc10();
void ai_Unk4c5970(int a, int b, int c);
void ai_Unk4c5b90(int a);

void he_Unk54fb50(int ev)
{
    int o = 0, r;

    gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
    if (o != 0 && *(int *)(o + 0x61) != 0 && (r = ai_Unk5ba620(*(unsigned short *)(ev + 0xc2))) != 0 && *(int *)(r + 4) != 0) {
        *(HETIME *)(ev + 0xb4) = ai_Time;
        gm_AddTime(ev + 0xb4, 0, *(int *)(r + 4), 0);
        *(int *)(ev + 0xb0) = 0;
        *(int *)(ev + 0x70) = 0;
        return;
    }
    he_Unk53e1f0((int *)ev);
}
