/* 0x56d2d0, the alloc of the research event 0x95: the character +0xac and its research record (0x4eaea0), the amount of its +4 (0x4eafa0) into +0xb4 / +0xb8, the clocks +0xbc / +0x52 = now, due 5 (0x4fe140), state 0, the building +0x16c of the character registered with 0x524260 (building, 0x4ffe90 (building's +0x5d, 2, 6, 0, 0x24b, character id, ev)); without a record the event is freed */

union AHMGRID * ahm_RandomRecipe();
int ahm_ResearchMinutes();
char *gm_Query(void *list, int n, ...);
void snd_PlayFavourComment(void);

void he_Unk56d2d0(int ev)
{
    char *c, *p;
    int v;

    c = (char *)gm_FindChar(*(int *)(ev + 0xac));
    if (c != 0) {
        p = (char *)ahm_RandomRecipe(c);
        if (p != 0) {
            *(int *)(ev + 0xb0) = *(int *)p;
            v = ahm_ResearchMinutes(c, p);
            *(int *)(ev + 0xb4) = v;
            *(int *)(ev + 0xb8) = v;
            *(HETIME *)(ev + 0xbc) = ai_Time;
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x70) = 0;
            v = gm_Query(*(int *)(*(int *)(c + 0x16c) + 0x5d), 2, 6, 0, 0x24b,
                             *(unsigned short *)c, ev);
            ((void (*)(int, int))sv_MoveToGroup)(*(int *)(c + 0x16c), v);
            snd_PlayFavourComment();
            return;
        }
    }
    he_Unk53e1f0((int *)ev);
}
