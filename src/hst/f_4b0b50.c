/* 0x4b0b50, the GESETZ_REL text command (callback 0x601b50[14], name 0x601070[14])
   "-PLUS_<decision>_n" / "-MINUS_...": moves the AI decision 0 / 15 (names of ai_CommandLaw) by n, clamped to its
   range. Sic: after the sign word it skips the length of the sign name with its own index.
   @deep B near: 204/204 instructions, only the kinds[2] slot left - the original keeps the array at [esp+0x70]/[esp+0x74], ours at [esp+0x68]/[esp+0x6c] (the 8-byte shift also moves the kinds[1] = 0xf store from before the byte store to after it: code hunk 4b0be7; the imul reads [esp+0x74] vs [esp+0x6c], slots hunk 4b0c48). Everything else is equal, including the max(min(d.max, i), d.min) operand order fixed this session and the early-return guard restructure (was a draft with the whole body in an if-block). Tried: int kinds[2] moved after AIDEC d - byte-identical, reverted. Slot allocation of a local array -> vargen / slotmap work. Q1 has evidence (callback 0x601b50[14], name 0x601070[14] GESETZ_REL), Q2/Q3/Q4/Q5/Q6/Q7 pass. */

int ai_CommandLawChange(int s, int b, char *fmt)
{
    if (*fmt != '-')
        return 0;
    {
        char sname[2][6] = { "MINUS", "PLUS" };
        char name[2][32] = { "RECHTSPRECHUNG_HAERTE", "AEMTEREINKOMMEN" };
        int kinds[2];
        int i, k;
        AIDEC d;

        s = -1;
        kinds[0] = 0;
        kinds[1] = 0xf;
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, sname[i], strlen(sname[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        if (i == 1)
            s = i;
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        k = kinds[i];
        if (!gv_GetTableRecord(k, &d))
            return 0;
        i = atoi(fmt + 1) * s + d.cur;
        gv_SetTableRecord(0, k, max(min(d.max, i), d.min));
        return 1;
    }
}
