/* 0x52c4e0, registers the object o (it must belong to the current scene: +0x88 == 0x5fb5b0) with the value b and the optional name in the table 0x761fc0 (a free entry, else the oldest expired one,
   time 0x761fc4 < 0x6333cc ms); the object's +0x228 callback becomes 0x52c4a0 */

void sv_Unk52c4e0(int o, int b, char *name)
{
    int i, j, *p;
    int *e;

    if (o != 0 && *(char **)(o + 0x88) == ai_Unk5fb5b0) {
        p = (int *)(unsigned)&sv_BubbleObjects;
        do {
            if (*p == o)
                return;
            p += 0x43;
        } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
        i = 0;
        p = (int *)(unsigned)&sv_BubbleObjects;
        do {
            if (*p == 0) {
                if (i < 0x40)
                    goto lfound;
                break;
            }
            p += 0x43;
            i++;
        } while ((int)p < ((unsigned)&sv_BubbleObjects + 0x4300));
        i = -1;
        j = 0;
        p = (int *)((unsigned)&sv_StatusText + 0x84);
        do {
            if (*p < SV_I((unsigned)&ai_Unk6333cc))
                i = j;
            p += 0x43;
            j++;
        } while ((int)p < ((unsigned)((unsigned)sv_StatusText + 0x88) + 0x42fc));
        if (i != -1) {
lfound:
            e = (int *)(i * 0x10c + ((unsigned)&sv_StatusText + 0x80));
            ((int *)((unsigned)&sv_StatusText + 0x84))[i * 0x43] = SV_I((unsigned)&ai_Unk6333cc);
            ((int *)(unsigned)&sv_BubbleObjects)[i * 0x43] = o;
            *e = b;
            *(int *)(o + 0x228) = (int)sv_ReleaseBubble;
            if (name != 0) {
                strcpy((char *)e + 0xc, name);
                return;
            }
            *(char *)(i * 0x10c + ((unsigned)&sv_BubbleObjects + 0x4)) = 0;
        }
    }
}
