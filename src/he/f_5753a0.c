/* 0x5753a0, the run of the event 0xa4: -2 / -1 free, also without the figure +0xc (0x505df0); a random 0x42ec20 picks: above
   0x5eb828 0x575430 (per), above 0x5eb7b8 0x575540 (per), else 0x575570 (per); then free */

void he_Unk575430(char *per);       /* 0x575430 */
void he_Unk575540(char *per);       /* 0x575540 */
void he_Unk575570(char *per);       /* 0x575570 */

void he_Unk5753a0(int ev)
{
    char *per;
    float r;
    int state;

    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    state = HM_I(ev, 0x70);
    if (state == -1 || state == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (per == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    r = util_RandomF();
    if (r > (0.800000012f))
        he_Unk575430(per);
    else if (r > (0.400000006f))
        he_Unk575540(per);
    else
        he_Unk575570(per);
    he_Unk53e1f0((int *)ev);
}
