/* 0x4f5a80, the wage report order (table 0x611768 entry 17; c the character, m the message, kinds 1 / 3) for
   characters with a profession (job / job2, 0x166 / 0x169) or the privilege flags 0xe00 of +0x1cc: a player
   (+2 = 6) with kind 3 gets the title text 0x1d96 through "$Z%s$N", then one 0x1d97 line per profession and per
   wage record of gm_CharacterWages (0x4fa700; the profession text 0x253 + the job, 0x27b with textVariant set,
   the amount at +4 / +8 / +0xc), shown through ord_PrivilegeDialog (mode 1); kind 1 writes the text 0x1d96 into
   m + 5; else 2
   @deep meaning settled and readable; only the callee-saved save placement differs: the original pushes esi at entry, ebx at 0x4f5ada after the two return-1 blocks and edi inside the kind-3 block, ours pushes ebx in the prologue (frame 0x410, 149 instructions and register roles equal); tried: declarations inside the kind-3 block, spell push-pop/order hints, early-return guards -> needs the register-allocation toolbox
   pushes esi in the entry block, ebx at 0x4f5ada (after the two return-1 blocks) and edi inside the kind-3
   block, and pops ebx before esi; ours pushes ebx in the prologue. The frame (0x410), the 149 instructions and
   the register roles (ebx = base, esi = c, edi = buf) are otherwise equal (tools/asmdiff.py). Tried and
   reverted: o / buf / base declared inside the kind-3 block (no change), spell push-pop / order hints, the
   early-return guards (the two return-1 blocks are already separate). Needs match-deep's register-allocation
   toolbox. */

int ord_WageReportOrder(char *c, int *m)
{
    if (m[0] != 3 && m[0] != 1)
        return 1;
    if (ORD_CHAR(c)->job == 0 && ORD_CHAR(c)->job2 == 0 && (ORD_CHAR(c)->flags1cc & 0xe00) == 0)
        return 1;
    if (m[0] == 3 && ORD_CHAR(c)->kind == 6) {
        ORDWAGES o;
        char buf[0x400];
        int base;

        gm_CharacterWages(ORD_CHAR(c)->id, 0, (unsigned char *)&o);
        memset(buf, 0, sizeof buf);
        base = (ORD_CHAR(c)->textVariant != 0 ? 0x27b : 0x253);
        txt_Format(buf, "$Z%s$N", 0x1d96);
        if (ORD_CHAR(c)->job != 0)
            txt_Format(buf + strlen(buf), 0x1d97, ORD_CHAR(c)->job + base, o.amount);
        if (ORD_CHAR(c)->job2 != 0)
            txt_Format(buf + strlen(buf), 0x1d97, ORD_CHAR(c)->job2 + base, o.amount2);
        if (o.kind != 0)
            txt_Format(buf + strlen(buf), 0x1d97, o.kind + base, o.amount3);
        ord_PrivilegeDialog(buf, 1, 0x657c7);
        return 2;
    }
    if (m[0] == 1 && ORD_CHAR(c)->kind == 6)
        txt_Format(m + 5, "%s", 0x1d96);
    return 2;
}
