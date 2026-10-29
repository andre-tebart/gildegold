/* 0x551340, steps one 16-byte slot (id, target id, 4 flag bytes) of the 0x5512c0 event: no id -> 0; the person (0x505df0) is looked up and its target (0x4fea50, out in the local): without a target a person that has an object (+0x184) is released (0x4c5ec0) and given the message 0x1c8 (4, 0, 0x40000000); with a target the walk is checked (0x524770) and while the person is still on its way (and the hour is before 0x16) the result is 2, else the slot is cleared (the person stops: 0x4c5230 / 0x4c5ec0, message 0x1c8) and the result 0 (ROUGH) */

void ai_Unk4c5ec0(int a);
void ai_Unk4c5230(int a);
int sv_Unk524770();
void cm_SendSetField(int a, int b, int c, int d, int e);

int he_Unk551340(int *s)
{
    int a, o = 0;
    unsigned char *b = (unsigned char *)s;

    if (s[0] == -1)
        return 0;
    a = gm_FindChar(s[0]);
    if (a != 0) {
        gm_FindById(&o, 0, 0, s[1]);
        if (o != 0) {
            if (sv_Unk524770(a, o) == 0 && *(unsigned short *)(unsigned)&ai_TimeHour < 0x16)
                return 2;
            if (*(int *)(a + 0x184) != 0) {
                ai_Unk4c5230(*(int *)(a + 4));
                ai_Unk4c5ec0(s[0]);
            }
            s[0] = -1;
            s[1] = -1;
            b[8] = 0;
            b[9] = 0;
            b[10] = 0;
            b[11] = 0;
            cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x40000000);
            return 0;
        }
        if (*(int *)(a + 0x184) != 0) {
            ai_Unk4c5ec0(s[0]);
            cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x40000000);
        }
    }
    s[0] = -1;
    s[1] = -1;
    b[8] = 0;
    b[9] = 0;
    b[10] = 0;
    b[11] = 0;
    return 0;
}
