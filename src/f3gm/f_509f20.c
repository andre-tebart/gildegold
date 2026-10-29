/* 0x509f20, the amount of the objects (prototype kind 9) of the character with the id of slot c, or -1 when it does not exist */

/* ---- layout 2026-10-23: the character / building worth helpers 0x509f20 .. 0x50af00 (ROUGH drafts) ---- */
int io_CharacterName();                     /* 0x5143d0 */
char * ai_Unk4cd9d0(int id);                     /* 0x4cd9d0 */
int gm_EquipmentValue(unsigned char *b, int unused);
int gm_GetWorth(unsigned short id);
int gm_ListAmount(short *l, char *o);

int gm_Unk509f20(int c)
{
    char *p;
    int s;
    char *o;

    p = gm_FindChar(*(int *)(ai_Chars[c] + 4));
    if (p != 0) {
        s = 0;
        o = gm_Query(*(void **)(p + 0x178), 1, 4, 9);
        while (o) {
            s += *(int *)(o + 0xe);
            o = (char *)gm_QueryNext();
        }
        return s;
    }
    return -1;
}
