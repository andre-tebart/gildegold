/* 0x47e990, runs the evaluation of a master: which function depends on the kind of its building @deep byte; readability left: Q1/Q2 the seven master-evaluation callees are undefined names - 0x47edb0 (the default "evaluates the production of a master's workshop"), 0x47f700 (the same for building kinds 0xe / 8 / 0x20), 0x4801f0 (the same for building category 2), 0x483fc0 (kind 0x1d), 0x484a60 (kind 0x1f), 0x483a60 (no role comment, kind 9), 0x4836a0 (the bank master's money balance) - the five production variants differ only by the AIENTRY flags their bodies test and the kind-to-profession table that would name them is in no source; 0x501d80 same (no source, no Ghidra export). Named on the way: 0x6f2308 -> ai_PlayerBuilding (ai.h), 0x6f230c -> ai_FallbackBuilding (the f3snd.c 2308-then-230c fallback), 0xf8a0c8 -> ai_LoggedMaster (ai.h "the master whose decisions are logged"). */

void ai_Unk47e990(AICHAR *c)
{
    char buf[256];
    AIWATCH *w;
    int t;
    char k;

    if (ai_TimeT.hour >= 6 && (ai_TimeT.hour != 6 || ai_TimeT.min >= 0x1e) &&
        ai_IsLocalJob(c) && (c->flags1b4 & 1)) {
        if (c == ai_LoggedMaster) {
            sprintf(buf, "Evaluation von Meister %s in %s beginnt...", ai_LoggedMaster->name,
                    ai_LoggedMaster->bld->name);
            sprintf(buf, "Aktuelles Budget: %li", ai_LoggedMaster->budget);
            sprintf(buf, "Tats\xe4" "chlich verf\xfc" "gbares Budget: %li",
                    gm_GetGoodsValue(&ai_CharsT[c->bld->owner]));
        }
        if (c->bld->owner == c->id)
            c->budget = gm_GetGoodsValue(c);
        if (c->bld == ai_PlayerBuildingT || c->bld == ai_FallbackBuildingT) {
            w = &ai_Watch;
            w->bld = c->bld;
            w->ch = c;
            w->owner = &ai_CharsT[c->bld->owner];
            w->budget = c->budget;
        }
        if (gm_BuildingClass(c->bld->type) != 1 && gm_BuildingClass(c->bld->type) != 4) {
            if (gm_BuildingClass(c->bld->type) == 2) {
                ai_EvaluateProductionCategory2(c);
                return;
            }
            k = ai_BTypesT[c->bld->type].kind;
            if (k == 0x13) {
                ai_CalcMeisterWache(c);
                return;
            }
            if (k == 4) {
                ai_CalcMeisterDiebe(c);
                return;
            }
            if (k == 0x10) {
                ai_CalcMeisterRaeuber(c);
                return;
            }
            if (k == 5) {
                ai_CalcMeisterBank(c);
                return;
            }
            if (k != 9)
                return;
            ai_Unk483a60(c);
            return;
        }
        k = ai_BTypesT[c->bld->type].kind;
        if (k != 0xe && k != 8 && k != 0x20) {
            if (k == 0x1d)
                ai_Unk483fc0(c);
            else if (k == 0x1f)
                ai_Unk484a60(c);
            else
                ai_EvaluateProduction(c);
        } else
            ai_EvaluateProductionFlags(c);
    }
}
