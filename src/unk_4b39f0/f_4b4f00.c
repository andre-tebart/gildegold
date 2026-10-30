/* 0x4b4f00, 1 if mission m is fulfilled (the condition of its type; 11 / 19 / 23 / 28 / 40: events counted by
   0x4b44e0) */

extern int ai_GameState;      /* 0x6f25ac, missions on */
int ai_Unk6f0728 = 0;  /* 0x6f0728 */

int ai_Unk4b4f00(G13MIS *m)
{
    G13TYPE *t;
    AICHAR *c;

    t = ai_GetMissionType(m->type);
    if (!ai_GameStateB || t == 0)
        return 0;
    if (ai_Unk6f0728 == 1 && ai_MissionMode >= 0 && ai_MissionMode <= 5) {
        ai_Unk6f0728 = 0;
        return 1;
    }
    c = gm_FindChar(m->owner);
    if (c == 0)
        return 0;
    switch (m->type) {
    case 1:
    case 6:
    case 14:
    case 22:
    case 32:
        return ai_Unk4b4550(c, t);
    case 2:
    case 7:
        return ai_Unk4b4590(c, t);
    case 3:
    case 10:
    case 24:
        return ai_Unk4b45c0(c, (int)m, t);
    case 5:
    case 13:
    case 21:
    case 30:
        return ai_Unk4b45f0(c, t);
    case 31:
        return ai_Unk4b4630(c, t);
    case 4:
    case 9:
    case 29:
        return ai_Unk4b4670(c, t);
    case 15:
    case 16:
    case 17:
    case 18:
    case 34:
    case 35:
        return ai_Unk4b46c0(c, (int)m, t);
    case 11:
    case 19:
    case 23:
    case 28:
    case 40:
        return m->unk1c >= t->goal;
    case 8:
        return ai_Unk4b46f0(c, t);
    case 12:
        return ai_HasThreeStockedBuildings(c, t);
    case 20:
    case 25:
    case 36:
        return ai_Unk4b4830(c, t);
    case 26:
    case 37:
        return ai_Unk4b4870(c, t);
    case 27:
        return ai_Unk4b48b0(c, t);
    case 41:
        return ai_CondCharShare(c, t);
    case 39:
        return ai_Unk4b4960(c, t);
    case 38:
        return ai_Unk4b4a00(c, t);
    case 33:
        return ai_Unk4b4aa0(c, (int)m, t);
    case 43:
        return ai_PlayerHeldBuildings(c, t);
    case 44:
        return ai_Unk4b4c50(c, (int)m, t);
    case 45:
        return ai_Unk4b4e30(c, t);
    case 46:
        return ai_Unk4b4dc0(c, (int)m, t);
    case 47:
        return ai_CondPlayerShare(c, t);
    case 48:
        return ai_Unk4b4d90(c, t);
    case 49:
        return ai_Unk4b4eb0(c, t);
    }
    return 0;
}
