/* 0x4d3390, the death of the character c when its strength 0x4fc8e0 is below 5edbd0: the model is stopped (0x415e90 / 0x405610),
   an object with the callback 0x4d3300 (blood puddle) and the sound name "kampf/todumfallen" is created (+0xf0), the character
   marked dead (+8 = 0), the sound "Schmerzschrei", a random text 0x1181 / 0x1182 sent to the side, the carried object (+0x1ac)
   put down and given to the defender; returns 1, else 0. Rough draft. */

int ai_Unk4d3390(char *c)
{
    char buf[0x100];
    char *m, *b, *p;
    int o;

    ai_Unk4cda50(c);
    if (!(unk_Unk4fc8e0(c) < ai_Unk5edbd0))
        return 0;
    d3t_CreateBlood(*(char **)(c + 0x184) + 0, 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
    ch_StopActions(*(char **)(c + 0x184));
    b = (char *)ch_AddActionCBK(*(char **)(c + 0x184), ai_Unk4d3300, 0x14, 0x36, 1);
    strcpy(b + 0xf0, "kampf/todumfallen");
    c[8] = 0;
    m = *(char **)(*(char **)(c + 0x184) + 0x34);
    *(int *)(m + 0x220) = 0;
    sb_PlaySample(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
    if (ai_Unk4e52f0(100) > 0x32) {
        *(int *)(c + 0x24) = (int)((double)(-(int)*(float *)(c + 0x1c)) - ai_Unk5eb8e0);
        G19_5176D0(buf, (char *)0x1181, *(unsigned short *)c);
    } else {
        *(int *)(c + 0x24) = (int)(ai_Unk5eb990 - (double)(int)*(float *)(c + 0x1c));
        G19_5176D0(buf, (char *)0x1182, *(unsigned short *)c);
    }
    o = *(int *)(c + 0x1ac);
    if (o != 0) {
        d3_SetWorldPosRot(o, m + 0x4c, m + 0x18c);
        d3if_Show(o, 1);
        *(int *)(c + 0x1ac) = 0;
        p = (char *)ai_Unk4ceb70(*(char **)ai_Unk6f0ce8);
        *(int *)(p + 0xa) = *(unsigned short *)ai_Unk6f0ce8;
    }
    sv_SetStatusText(buf);
    return 1;
}
