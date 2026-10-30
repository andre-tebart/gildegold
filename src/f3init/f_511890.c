/* 0x511890, gives every building of the class 6 (not the type 0x1c) a name: the names of its kind (the 0xc names at 0x143b400 + 0x38 per kind) that no building uses yet, up to 0x20 characters ("Building name too long: %s" is the error text), one picked at random into the building's name field (+5) @deep behaviour: 178 code hunks / 404 of 544 bytes, meaning read whole against the original 0x511890..0x511ad0 (both gm_BuildQuery(1,6) walks, the 0xc x 0xe name table walk, the 0x100 x 0xa9 ai_Blds compare, the 0x20 length check, the random pick, the two error formats) - no meaning difference left; the frame now matches 0x810 exactly. Frame finding of this session: the original has TWO 0x200-byte error buffers, one per "Building name too long" format (0x120..0x320 for the arm that calls err_Message, 0x620..0x810 for the arm that drops the text); our single err[0x200] left the frame 0x610 and every buffer 0x200 too low. After adding the second (char text[0x200]) the anchors match: one at 0x20, names at 0x320, the sprintf/err_Message pair at 0x124/0x12c, the dead format at 0x620. Left (allocation only): the original pushes ebx/ebp/edi mid-function (at 0x5118ee, the second walk) where ours pushes them in the prologue, the first walk keeps cnt/zero in esi vs the original's [esp+0xc] + test eax, and the register assignment of the strcpy/strlen blocks (109 +/- lines, every one read). Also tried and reverted: util_Random((unsigned char)cnt) - the original zero-extends the count with xor dx,dx / mov dl,al (a char parameter in its TU), our cast gives and eax,0xff and 179 code hunks, plain cnt gives 178. */


int init_NameBuildings(void)
{
    unsigned char *b;
    char names[0xc][0x40];
    int cnt, i, j, k;
    char *src, *u;
    char one[0x100];
    char err[0x200];            /* the "Building name too long" text handed to err_Message */
    char text[0x200];           /* the same text of the first (dead) check: the original formats it and drops it */
    unsigned r;
    char *bl;

    b = (unsigned char *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (ai_BTypes[*b * 0x24d] != 0x1c)
            b[5] = ch_EmptyName[0];
        b = (unsigned char *)gm_BuildNext();
    }
    b = (unsigned char *)gm_BuildQuery(1, 6);
    while (1) {
        if (b == 0)
            return 0;
        if (ai_BTypes[*b * 0x24d] != 0x1c) {
            cnt = 0;
            b[5] = ch_EmptyName[0];
            for (i = 0; i < 0xc; i++) {
                src = ai_BuildingNames[i + *b * 0xe];
                if (*src != 0) {
                    strcpy(one, src);
                    bl = ai_Blds;
                    for (j = 0; j < 0x100; j++, bl += 0xa9) {
                        if (*bl != 0 && strcmp(bl + 5, one) == 0)
                            break;
                    }
                    if (j < 0x100)
                        continue;
                    if (strlen(one) < 0x20) {
                        strcpy(names[cnt], one);
                        cnt++;
                    } else
                        sprintf(text, "Building name too long: %s", one);
                }
            }
            if (cnt != 0) {
                r = util_RandomU(cnt);
                u = names[r & 0xffff];
                if (strlen(u) < 0x20)
                    strcpy((char *)b + 5, u);
                else {
                    sprintf(err, "Building name too long: %s", u);
                    err_Message(err);
                }
            }
        }
        b = (unsigned char *)gm_BuildNext();
    }
}
