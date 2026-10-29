/* 0x497ee0, execute of 0x497cf0: pays for the renovation */

int ai_Unk497ee0(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG msg;
    AIFINX *fin;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x1c;
    msg.to = p->netId;
    msg.fromId = -1;
    msg.flags = 1;
    msg.arg[0] = s1->unk04;
    msg.arg[1] = s1->unk10;
    cm_BeginGroup("renovieren");
    cm_SendMoveGoods(-1, s2->unk04, ai_OwnPlayer, (int)s1->unk14);
    cm_SendLongMessage(&msg);
    cm_EndGroup("renovieren");
    fin = (AIFINX *)gm_PlayerRecord((AICHAR *)p);
    if (fin)
        fin->renovationSpent = (int)(fin->renovationSpent + s1->unk14);
    return 15;
}
