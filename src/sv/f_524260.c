/* 0x524260, moves the character c (index) from its current group record (+0x17c) to the group g and the item it, by the kinds of the groups: tells the old group's owner (the messages 0x33 / 0x39 with the tag
   "sv_ChangePlayerHE" when the new item differs and the time of day is in the window of 0x617830 / 0x617840 or the building is of kind 4 / 0x13 / 0x1d), removes c from the old group (0x524160), finishes it by its kind
   (the switch: 3 / 4 / 8 / 9 0x5241d0, 0x14 / 0x15 / 0x8c count and reset the clock, 0x16 ... 0x53e1f0, 0x28 the pause, 0x48 / 0x49 / 0x75 / 0xa1 empty groups are closed 0x4c4c60), adds c to the new group
   (0x524100), and sends the group message (0x4c3ba0 / 0x4c3d90 / 0x4c49e0 with the kind of the new group) when the group changed. ROUGH */

extern float ai_Unk617830[], ai_Unk617840[];   /* 0x617830 / 0x617840 */
int init_BuildingSceneUniverse();                     /* 0x511e60 */
int init_ObjectSceneUniverse();                     /* 0x511dc0 */
int gm_ClockSeason(void *t);           /* 0x4fe350, f3gm.c */                     /* 0x4fe350 */
struct CHACTION * ch_AddAction(struct CHARACTER *ch, unsigned int type, ...);                   /* 0x40b2f0 */
char *ai_Unk5de6b0();                   /* 0x5de6b0, strstr (LIBC; as ls.c) */
void cm_SendExSetHE(unsigned char k, char *p);                     /* 0x4c4c60 */
void he_Unk53e1f0();                     /* 0x53e1f0 */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                     /* 0x4c3d90 */
void cm_SendFieldValues(void);                     /* 0x4c49e0 */

void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g)
{
    char *ch, *old, *msg, *ev, *b;
    int w, k;
    char kind;
    char newkind;

    if (c >= 0x300)
        return;
    ch = ai_Chars[c];
    old = *(char **)(ch + 0x17c);
    if (old == g)
        return;
    if (o == 0)
        o = *(unsigned char **)(ch + 0x16c);
    if (it != 0) {
        b = ai_BTypes + *o * 0x24d;
        if (*it == 0xfd)
            w = init_BuildingSceneUniverse(o);
        else
            w = init_ObjectSceneUniverse(it);
        k = gm_ClockSeason(ai_Time);
        if ((((float)*(unsigned short *)(ai_Time + 4) < ai_Unk617840[k] && ai_Unk617830[k] <= (float)*(unsigned short *)(ai_Time + 4)) || b[0] == 4 || b[0] == 0x13 || b[0] == 0x1d)
            && w > -1 && *(int *)(ch + 0x184) != 0 && (char *)(unsigned)&d3_Scenes + w * 0x3e0 != *(char **)(*(int *)(ch + 0x184) + 0x88)) {
            msg = ch_AddAction(*(int *)(ch + 0x184), 0x33, w, *(int *)(o + 1), *(int *)(it + 2));
            strcpy(msg + 0x70, "sv_ChangePlayerHE");
            msg = ch_AddAction(*(int *)(ch + 0x184), 0x39, w);
            strcpy(msg + 0x70, "sv_ChangePlayerHE");
            if (*(int *)(*(int *)(ch + 0x184) + 0x28) != -1) {
                ev = (int)evt_FindById(*(int *)(*(int *)(ch + 0x184) + 0x28));
                if (ev != 0)
                    evt_Stop((EVTSCRIPT *)ev);
            }
        }
    }
    if (old == 0) {
        if (*(int *)(ch + 0x184) != 0 && *(int *)(*(int *)(ch + 0x184) + 0x28) != -1) {
            ev = (int)evt_FindById(*(int *)(*(int *)(ch + 0x184) + 0x28));
            if (ev != 0 && (ai_Unk5de6b0(ev, "Pause") != 0 || ai_Unk5de6b0(ev, "pause") != 0))
                evt_Stop((EVTSCRIPT *)ev);
        }
    } else {
        sv_RemoveGroupMember(*(unsigned short *)ch, old);
        switch ((unsigned char)old[0]) {
        case 3:
        case 4:
        case 8:
        case 9:
            sv_CountGroupMembers(old);
            break;
        case 0x14:
        case 0x15:
        case 0x8c:
            sv_CountGroupMembers(old);
            if (sv_UsedGroupSlots(old) < 1) {
                *(int *)(old + 0x52) = *(int *)ai_Time;
                *(int *)(old + 0x56) = *(int *)(ai_Time + 4);
                *(int *)(old + 0x5a) = *(int *)(ai_Time + 8);
                *(short *)(old + 0x5e) = *(short *)(ai_Time + 0xc);
                cm_SendExSetHE(-1, old);
            }
            break;
        case 0x16:
        case 0x88:
        case 0x89:
        case 0x8d:
        case 0x8f:
            he_Unk53e1f0(old);
            break;
        case 0x28:
            if (*(int *)(ch + 0x184) != 0 && *(int *)(*(int *)(ch + 0x184) + 0x28) != -1)
                evt_Stop(evt_FindById(*(int *)(*(int *)(ch + 0x184) + 0x28)));
            if (*(int *)(old + 0x70) < 3) {
                *(int *)(old + 0x52) = *(int *)ai_Time;
                *(int *)(old + 0x56) = *(int *)(ai_Time + 4);
                *(int *)(old + 0x5a) = *(int *)(ai_Time + 8);
                *(int *)(old + 0x70) = -1;
                *(short *)(old + 0x5e) = *(short *)(ai_Time + 0xc);
            }
            break;
        case 0x48:
        case 0x49:
        case 0x75:
        case 0xa1:
            if (sv_UsedGroupSlots(old) < 1)
                cm_SendExSetHE(-1, old);
        }
    }
    if (g == 0)
        newkind = 0;
    else {
        sv_Unk524100(*(unsigned short *)ch, g);
        switch ((unsigned char)g[0]) {
        case 3:
        case 4:
        case 8:
        case 9:
        case 0x14:
        case 0x15:
        case 0x8c:
            sv_CountGroupMembers(g);
        }
        newkind = g[0];
    }
    if ((*(char **)(ch + 0x17c) != 0 && g == 0) || newkind != 0) {
        cm_BeginFieldBlock(ch, *(int *)(ch + 4));
        cm_AddFieldValue(1, 1, 0x180, &newkind);
        cm_SendFieldValues();
        *(char **)(ch + 0x17c) = g;
    }
}
