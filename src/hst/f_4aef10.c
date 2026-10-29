/* 0x4aef10, the REICHSTER_EINWOHNER token of a history text (callback 0x601b10[6], name 0x601730[6]): the
   richest character (0x50a640) who passes 0x4adff0 / 0x4adfb0; fmt "...-n" selects 0 = anybody, 1 = men
   (byte 9 == 0 .. sic: byte 9 == (n != 1)), 2 = the other sex
   @deep behaviour: far draft, 177 code hunks left; read: the sel parse (sel 0..2, sel != 0 sets sex/filt) and the mode 1/0/2 paths match the reading, the difference is the frame and slot layout (ours 0x214, original 0x210: our locals best/sex/filt/n1/n2 get separate slots, the original keeps filt in [ebp-4]=0x200, sex in [ebp-0x10], n1 in [ebp-8] and the second out in the dead fmt slot [ebp+0x14]) plus the loop and the worth-comparison instruction order; the AI_ZERO dst here is filt (a variable) and still materialises. Q3 fixed this session by naming the -10000000 start value HST_NO_WORTH; Q1 name has evidence (callback 0x601b10[6] = 0x4aef10, name 0x601730[6] REICHSTER_EINWOHNER). Leads left for deep: the slot allocation of filt/sex/best (a slot-allocation problem like ai_TokenRandomGuildMaster's _zp temp) and the loop form */

/* below every character's worth (gm_GetWorth): the "no candidate yet" start value of the richest searches */
#define HST_NO_WORTH (-10000000)

int ai_TokenRichestInhabitant(int mode, G12ARG *args, int k, char *fmt, char *out)
{
    char buf[512];
    int n1, n2;
    int sel, i, v;
    int best = HST_NO_WORTH;
    unsigned int sex = 0;
    unsigned int filt = 0x200;
    AICHAR *c;

    n1 = (int)buf;
    AI_ZERO((char *)n1, filt);
    ai_SplitTokenFormat(fmt, buf, &n1, &n2);
    if (n2 == 1) {
        sel = atoi(fmt + n1);
        if (sel >= 0 && sel < 3) {
            if (sel != 0)
                sex = sel != 1;
            filt = sel != 0;
            if (mode != 2) {
                c = 0;
                if (mode == 1) {
                    c = gm_FindChar(args[k].id);
                    if (c != 0 && c->health != 0) {
                        txt_FormatString(out, buf, c->id);
                        return 1;
                    }
                } else {
                    for (i = 0; i < 0x300; i++) {
                        if (ai_IsLivingCharacter((unsigned short)i) && (filt == 0 || G12_B(i, 9) == sex)
                            && (v = gm_GetWorth((unsigned short)i)) > best && ai_NotInLists(ai_Chars[i].netId)) {
                            c = &ai_Chars[i];
                            best = v;
                        }
                    }
                    if (c != 0) {
                        if (mode == 0)
                            args[k].id = c->netId;
                        txt_FormatString(out, buf, c->id);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
