/* 0x59e210, whether the player may enter the building param_1 (528 B): the kind byte (0x7e3f30 + *param_1 *
   0x24d), 0x501d80 (the kind), 0x503530 (player, the building, 0, 0); no entry with the byte +0x5b & 4, yes
   with the network flag 0x6f23f8 < 0, with the "own" byte 0x770d31 + player * 0x21c and the kinds 4 / 6, with
   the flag 0x200 or the player's own building (param_1 + 0x27); the flag 1 needs the open hours 0x59e170,
   else the texts 0x2056 / 0x2057 (0x2059 / 0x205a for the kinds 3 / 5) and the hours text 0x2058 (0x521410
   (the global 0x1442314 text, the message, 4. ROUGH: the string at 0x1442314 is a pointer) */

int ai_Unk59e210(unsigned char *param_1)
{
    char kind;
    int k;
    unsigned int f;
    unsigned int from;
    unsigned int to;
    unsigned short slot;
    char buf[0x80];
    char date[0x80];
    char msg[0x400];
    int id;

    kind = *(char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d);
    k = gm_BuildingClass(*param_1);
    f = unk_Unk503530(U_I((unsigned)&ai_PlayerId), param_1, 0, 0);
    if ((param_1[0x5b] & 4) != 0)
        return 0;
    if (*(signed char *)(unsigned)&ai_GameFlags < 0)
        return 1;
    if (*(unsigned char *)(((unsigned)&ai_Selected + 0x29) + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0 && (kind == 6 || kind == 4))
        return 1;
    if ((f & 0x200) != 0)
        return 1;
    slot = *(unsigned short *)(param_1 + 0x27);
    if (slot == U_W((unsigned)&ai_PlayerId))
        return 1;
    if ((f & 1) == 0)
        return 0;
    if (ai_Unk59e170(param_1, &from, &to) != 0)
        return 1;
    strcpy(buf, (char *)U_I((unsigned)&full_data_1442314));
    if ((int)(U_I((unsigned)&ai_TimeHour) & 0xffff) < (int)from) {
        if (k != 3 && k != 5) {
            txt_Format(msg, 0x2056, slot, param_1);
            goto out;
        }
        id = 0x2059;
    } else {
        if (k != 3 && k != 5) {
            txt_Format(msg, 0x2057, slot, param_1);
            goto out;
        }
        id = 0x205a;
    }
    txt_Format(msg, id, param_1);
out:
    txt_Format(date, 0x2058, from, to);
    strcat(msg, date);
    U_I((unsigned)&sv_PanelFlag) = 0;
    sv_MessageBoxGreen(buf, msg, 4);
    return 0;
}
