/* 0x4f4f20, an order handler (c the character, m the message, kind 1 or 3): characters of level 3 or more: 4; a player (+2 = 6) with kind 3 gets the text 0x1d34 (level + 0x114, + 10 with +9) shown through 0x4f8200; kind 1 writes text 0x1d33 into m + 0x14; 1 for other kinds, else 2 @deep meaning settled and readable; remaining diff is allocation/scheduling only (97/208 bytes, 38 code bytes): the original keeps m in esi and r in edi (ours m = edi, r = esi, the same rank order), its r = 4 return block sits at 0x4f4fad between the kind-3 text block and the kind-1 text block (ours puts it after the if/else), and the r = 2 setup order differs. Won by the last-bytes pass: the complete-constant ternary (c[9] != 0 ? 0x11e : 0x114) + level instead of (c[9] ? 10 : 0) + 0x114 + level gives the original's and eax,0xa; add eax,0x114; add eax,ecx (notes/vc6-matching.md round entry; the same spelling is owed to the other level/job handlers, 0x253 variants: (c[9] ? 0x27b : 0x253) + job), and dropping the unsigned char lv local (read c[0xd] twice) keeps the byte load in cl and removes the 4-byte frame slot. Tried and reverted: level check forms (>=3 with r=4 inline, nested if/else, inverted <3 with else r=4, early return 4), r = 2 before/after the level check, a single final return r vs early returns, declaration order of r and buf. The 0x4f4ff0 idiom and the else-if chain gave the best byte counts. Needs match-deep's allocation toolbox (vargen ren / the push-pop: block the callee-saved register crosses lead). */

int ord_LevelOrder(char *c, int *m)
{
    char buf[0x100];
    int r;

    if (m[0] != 3 && m[0] != 1)
        return 1;
    if ((unsigned char)c[0xd] >= 3) {
        r = 4;
    } else {
        r = 2;
        if (m[0] == 3 && c[2] == 6) {
            txt_Format(buf, 0x1d34, (c[9] != 0 ? 0x11e : 0x114) + (unsigned char)c[0xd]);
            ord_PrivilegeDialog(buf, 0, 0x657c7);
        } else if (m[0] == 1 && c[2] == 6)
            txt_Format(m + 5, "%s", 0x1d33);
    }
    return r;
}
