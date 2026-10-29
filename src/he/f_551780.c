/* 0x551780, steps one 16-byte slot of the 0x5512c0 event (a person on a walk through town; bytes: +8 the stage counter, +9 flags 0x40 / 0x10 / 0x04 (done), +0xa the step, +0xb the building index, +0xc the hour it started): the person (0x505df0) is gone -> the slot is cleared; with a target (0x4fea50) the walk is checked (0x524770) and a person with a work object (+0x184) is held / released by the hour (message 0x1c8 / 0x1000, 0x4c5b90); every 1 + budget / 2 stages (budget = 2 + random 0x42ec00(6) + the two flags) the person plays an animation (0x5516a0); else a new target building is looked for, with a chance above 0.5 from the 0x20 buildings of the list 0x501190 (1, 5, 0x16) through 0x55ee40, else by walking the 0x100 buildings with step +0xa for one of the kinds in the mask 0xa7f4c3a0; the found building number goes to +0xb, its id to +4, and the person is sent there (0x4c52e0 "...", the flag differs for the kinds 0x16 / 7 / 0xf) (ROUGH) */

unsigned char *gm_BuildQuery(int n, ...);
unsigned char * gm_BuildNext(void);
char *gm_FindBuildingById(int id);
int he_Unk55ee40();
void ai_Unk4c5b90(int a);
int ai_Unk4c52e0();

void he_Unk551780(unsigned char *s)
{
    int v10, o = 0, v18 = 0, a;
    int arr[0x20];
    int n, bld, kind, step, ebx, left;
    unsigned lo, hi;
    char c;
    int obj;

    a = gm_FindChar(*(int *)s);
    if (a == 0) {
        *(int *)s = -1;
        *(int *)(s + 4) = -1;
        s[8] = 0;
        s[9] = 0;
        s[10] = 0;
        s[11] = 0;
        s[12] = 0xff;
        return;
    }
    gm_FindById(&o, 0, 0, *(int *)(s + 4));
    v18 = 0;
    if (o == 0)
        goto cleanup;
    v10 = (U_I((unsigned)&ai_Unk76061c) != 0) + (U_I((unsigned)&ai_Unk760618) != 0 ? 2 : 0) + (unsigned short)util_Random(6) + 2;
    if (sv_Unk524770(a, v18) == 0)
        return;
    obj = *(int *)(a + 0x184);
    if (obj != 0 && *(int *)(obj + 0x2c) != -1 && *(int *)(obj + 0x30) != -1) {
        c = (char)s[0xc];
        s[9] |= 0x40;
        v18 = 1;
        if (c == 0xff) {
            s[0xc] = (unsigned char)*(unsigned short *)(unsigned)&ai_TimeHour;
            cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x1000);
            return;
        }
        if ((*(unsigned short *)(unsigned)&ai_TimeHour & 0xffff) <= c + 1)
            return;
        s[9] &= 0xbf;
        cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x1000);
        ai_Unk4c5b90(*(int *)(a + 4));
        s[0xc] = 0xff;
    }
    n = s[8];
    if (10 - n < v10) {
        s[9] |= 4;
        return;
    }
    if ((s[9] & 0x10) == 0) {
        step = v10 / 2 + 1;
        if ((n + 2) % step == 0 && v18 == 0) {
            s[8] = (unsigned char)(n + 1);
            he_Unk5516a0(a);
            s[9] |= 0x10;
            return;
        }
    }
cleanup:
    s[9] &= 0xef;
    ai_Unk4c5230(*(int *)(a + 4));
    if ((float)util_RandomF() > 0.5f) {
        int e = gm_BuildQuery(1, 5, 0x16);
        int i = 0;

        if (e != 0) {
            while (i < 0x20) {
                if (*(unsigned short *)(e + 0x27) != 0xffff && *(int *)(e + 0x61) != 0)
                    arr[i++] = e;
                e = gm_BuildNext();
                if (e == 0)
                    break;
            }
            if (i != 0 && he_Unk55ee40(a, i, arr, &v10, &v18) != 0)
                gm_FindBuildingById(v10);
        }
        bld = o;
    } else {
        step = s[10];
        ebx = (s[11] + step) & 0xff;
        left = 0x100;
        bld = 0;
        do {
            bld = U_I((unsigned)&ai_Blds) + ebx * 0xa9;
            o = bld;
            if (*(unsigned char *)bld != 0) {
                int k = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)bld * 0x24d);

                lo = 0;
                hi = 0;
                if (k > 0x1f)
                    hi = 1u << (k - 0x20);
                else
                    lo = 1u << k;
                if (*(int *)(bld + 0x61) != 0 && ((lo & 0xa7f4c3a0) != 0 || (hi & 1) != 0))
                    break;
                bld = 0;
                o = 0;
            }
            ebx = (ebx + step) & 0xff;
            left--;
        } while (left != 0);
    }
    if (bld == 0) {
        s[9] |= 4;
        return;
    }
    s[11] = (unsigned char)ebx;
    *(int *)(s + 4) = *(int *)(bld + 1);
    s[8]++;
    kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)bld * 0x24d);
    if (kind == 0x16 || kind == 7 || kind == 0xf)
        ai_Unk4c52e0(*(int *)s, *(int *)(s + 4), -1, 0, 0, (char *)((unsigned)&ai_Unk625820 + 0x17c8));
    else
        ai_Unk4c52e0(*(int *)s, *(int *)(s + 4), -1, 0, 1, (char *)((unsigned)&ai_Unk625820 + 0x17c8));
}
