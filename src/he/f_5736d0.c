/* 0x5736d0, dispatches the approach +0xac `a` of the event 0x9d: 0 0x573760, 1 0x573820, 2 0x573970, 3 0x573ae0, 4 0x573c20, else 0x573d30 */

void he_Unk573760(char *pl, char *per);     /* 0x573760 */
void he_Unk573820(char *pl, char *per);     /* 0x573820 */
void he_Unk573970(char *pl, char *per);     /* 0x573970 */
void he_Unk573ae0(char *pl, char *per);     /* 0x573ae0 */
void he_Unk573c20(char *pl, char *per);     /* 0x573c20 */
void he_Unk573d30(char *pl, char *per);     /* 0x573d30 */

void he_Unk5736d0(char *pl, char *per, int a)
{
    if (a == 0)
        he_Unk573760(pl, per);
    else if (a == 1)
        he_Unk573820(pl, per);
    else if (a == 2)
        he_Unk573970(pl, per);
    else if (a == 3)
        he_Unk573ae0(pl, per);
    else if (a == 4)
        he_Unk573c20(pl, per);
    else
        he_Unk573d30(pl, per);
}
