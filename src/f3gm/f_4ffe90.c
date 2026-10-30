/* 0x4ffe90, starts an object query over the list (or, with the flag 7, over the whole instance table) and returns the first match (0x4ffcc0): n criteria follow as (kind, value) pairs: kind 0 = prototype, 1 = owner id, 3 = the 0x50a0b0 value, 4 = the prototype kind (each with a value), 5 = stop at the first match, 6 = descend into the children, 7 = scan the whole table (without value). Not set criteria match everything; 0 without a table or list. Used as gm_Query (list, 2, 6, 0, 0x115) .. all over the exe */

char *gm_Query(void *list, int n, ...)
{
    int *ap;
    char scan, kids;
    int i;

    if (ai_Goods != 0) {
        scan = 0;
        kids = 0;
        ai_Unk61a67c = 0x7fff;
        ai_Unk61a680 = -1;
        ai_Unk61a684 = -1;
        ai_Unk61a686[0] = (char)0xff;
        ai_Unk6f1bea = 0;
        ai_Unk6f1be8 = 0;
        ai_Unk6f1be9 = 0;
        ai_Unk6f1be0 = 1;
        ai_Unk6f1be4 = 0;
        ai_Unk7e3f38 = 0;
        if (n > 0) {
            ap = &n + 1;
            for (i = n; i != 0; i--) {
                switch ((char)*ap) {
                case 0:
                    ai_Unk61a67c = (unsigned short)ap[1];
                    ap += 2;
                    break;
                case 1:
                    ai_Unk61a680 = ap[1];
                    ap += 2;
                    break;
                case 3:
                    ai_Unk61a684 = (short)ap[1];
                    ap += 2;
                    break;
                case 4:
                    ai_Unk61a686[0] = (char)ap[1];
                    ap += 2;
                    break;
                case 5:
                    ai_Unk6f1bea = 1;
                    ap += 1;
                    break;
                case 6:
                    kids = 1;
                    ap += 1;
                    break;
                case 7:
                    scan = 1;
                    ap += 1;
                    break;
                default:
                    ap += 1;
                }
            }
            ai_Unk6f1be8 = scan;
            ai_Unk6f1be9 = kids;
            if (scan != 0) {
                if (kids != 0) {
                    ai_Unk6f1be0 = 1;
                    ai_Unk6f1be4 = 0;
                    ai_Unk7e3f38 = 0;
                    return 0;
                }
                ai_Unk6f1bf4 = (unsigned short *)ai_ObjectsV;
                return (char *)gm_QueryNext();
            }
        }
        if (list != 0) {
            ai_Unk6f1bf4 = (unsigned short *)list;
            return (char *)gm_QueryNext();
        }
    }
    return 0;
}
