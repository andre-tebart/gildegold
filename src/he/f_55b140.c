/* 0x55b140, the run handler of the random theft message event (ROUGH): a flagged (2) record with a pending handle +0x84 waits (0x4c3ae0); states -2 .. -1 and above 0 free the record; state 0 with +0xb8 >= 2 looks at the building +0xac (not of the kinds 0xb / 0xc / 0xd; the type 0x27 needs a player owner of kind 6 / 7 at +0x27), rolls a chance from the building's value (0x509b10 (building, +0x5c), the factor 0x5ebb44 and +0xb0) against 0x42ec00(100) and sends a message record (type 0x6e, +0x4a = 1) through 0x4c4b70, then posts the event (0x4c4c60 (0, ev)) with the chance for a second round (0x4c4a60) */

void he_Unk55b140(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, kind, o, r;
    unsigned char m[0xf8];
    float chance;

    if ((*(unsigned char *)(ev + 0x78) & 2) != 0 && *(int *)(ev + 0x84) != -1) {
        if (cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
            return;
        *(int *)(ev + 0x84) = -1;
    }
    if (st < -2 || st > 0 || st != 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0xb8) >= 2) {
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d);
        if (kind == 0xd || kind == 0xc || kind == 0xb) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        o = gm_EquipmentValue(b, *(char *)(b + 0x5c));
        chance = (float)*(int *)(ev + 0xb0) / ((float)o + *(float *)(unsigned)&full_rdata_5ebb44);
        r = (unsigned short)util_Random(100);
        if ((float)r <= chance * (50.0f)) {
            memset(m, 0, sizeof(m));
            m[4] = 0x6e;
            *(int *)(m + 0xc) = -1;
            m[0x36] = 1;
            *(int *)(m + 8) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c);
            cm_SendLongMessage(m);
        }
    }
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
