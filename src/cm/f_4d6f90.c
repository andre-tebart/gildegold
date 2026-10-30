/* 0x4d6f90, the cheer of the winner in the slot f: event 0x4d3ac0 with the animation "gestik/jubeln" */

void ai_Unk4d6f90(char *f)
{
    char *c, *ev;

    if (*(int *)(f + 4) != 0 && *(int *)f != -1) {
        c = ai_Unk4ce130(*(int *)f);
        ev = (char *)ch_AddActionCBK(*(char **)(c + 0x184), (unsigned)&ai_Unk4d3ac0, 5, 0x2e, 1);
        *(char **)(ev + 0x17c) = c;
        strcpy(ev + 0xf0, "gestik/jubeln");
    }
}
