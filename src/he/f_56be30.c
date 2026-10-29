/* 0x56be30, the run handler of the building construction event (the type 0xd; ROUGH SKELETON: the states 0x70 = -2 .. 7 from the jump table 0x56cda8, the calls in order from the disassembly; the conditions and most call arguments are not recovered yet; the states -1 / -2 hand the building back, rebuild its scene object and free the record) */

int gm_CompareTime();
short * gm_FindChar(int id);
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);
short *gm_PlayerRecord(char *c);
int ai_Unk50c4e0();
void ai_Unk4c5510(int a);
void cm_SendExSysMessage(unsigned char id, void *msg);

void he_Unk56be30(int ev)
{
    int o;

    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    switch (*(int *)(ev + 0x70)) {
    case -2:
    case -1:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xb8));
        d3if_SelectScene(0, 1);
        if (*(int *)(ev + 0xbc) != -1) {
            o = (int)evt_FindById(*(int *)(ev + 0xbc));
            if (o != 0)
                evt_Stop((EVTSCRIPT *)o);
            *(int *)(ev + 0xbc) = -1;
        }
        *(int *)(ev + 0xc8) = *(int *)(ev + 0xc4) + 1;
        he_ShowConstruction(ev);
        if (*(unsigned char *)(ev + 0x78) & 2)
            ((void (*)(int))cm_SendExSysMessage)(0);
        d3if_SelectScene(U_I((unsigned)&d3_CurrentScene), 1);
        he_Unk53e1f0((int *)ev);
        break;
    case 0:
        /* 0x4fe210 (ev + 0x52, &ai_Time); 0x505df0 (ev + 0xac); 0x4c47e0 (4 args) -> +0xcc; 0x506430; 0x50c4e0 (ev + 0xb4) */
        break;
    case 1:
        if (*(unsigned char *)(ev + 0x78) & 4)
            break;
        if (cm_GetSlotState(*(int *)(ev + 0xcc)) == 0)
            break;
        if (cm_GetSlotState(*(int *)(ev + 0xcc)) == 2)
            cm_SendExSetHE(-1, ev);
        break;
    case 2:
        /* 0x4c45c0 (4 args); 0x4c4c60 */
        break;
    case 3:
        /* 0x4c3b10; 0x4c4c60 */
        break;
    case 4:
        /* 0x4fea50 (out, 0, 0, id); 0x56bc50 (rot, pos, type, model, ev); 0x4c4c60; 0x4478e0 */
        break;
    case 5:
        /* 0x50d380; 0x56bac0 x2; 0x4fe140; 0x4478e0; 0x4fea50; 0x58fc40; 0x458a20; 0x449690; 0x4c4c60; 0x438d40; 0x4287b0; 0x429ab0; 0x42ada0 */
        break;
    case 6:
        /* 0x4fe140; 0x4fea50; 0x58fc40; 0x4c4c60; 0x438d40; 0x458780; 0x447190 */
        break;
    default:
        /* state 7: see the disassembly 0x56c5a9 .. 0x56ccf2 (the long final part) */
        break;
    }
}
