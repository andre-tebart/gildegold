/* 0x4d3550, the hit of the fight event ev (+0x17c the attacker, +0x180 the target, +0x184 its record): the weapon range test
   0x402140 / 0x4ce1b0, the worker count of the player's characters synced (+0x20 -> field), damage 0x4cedc0, the pain
   speech "_K_C2C_SCHMERZ" or the sound "Schmerzschrei", the hit / miss sound by the attacker's kind ("schuss_s", "Treffer",
   "Daneben_s"), 0x4d3390 for a death, else the target's model flinches (0x415e90 / 0x405610 / 0x405810 with
   "kampf/stich_abwehr" / "kampf/ausweichen_seite", the text 0x1188 for the kind 0x16e). Rough draft. */

#define G19_FIGHTER_SYNC(ch)                                                          \
    r = ai_Unk4cdac0(ch);                                                             \
    if (r != 0) {                                                                     \
        vv = -1;                                                                      \
        cm_BeginFieldBlock(r, *(int *)(r + 2));                                             \
        cm_AddFieldValues(4, 1, (unsigned short)(r - cm_FieldObject + 0x20), &vv);           \
        cm_SendFieldDeltas();                                                               \
    }

void ai_Unk4d3550(char *ev)
{
    char buf[0x80];
    char tbl[0x14];
    char *att = *(char **)(ev + 0x17c);
    char *tgt = *(char **)(ev + 0x180);
    char *ka, *kt, *r;
    int vv, hit, rnd, a, b;
    int hitv = 0;
    int kt1 = 0;

    ka = ai_Unk4cda50(att);
    kt = ai_Unk4cda50(tgt);
    ch_GetHMap(tbl, att, tgt);
    if (kt != 0 && ai_Unk4ce1b0(tgt, *(char **)(*(char **)(att + 0x184) + 0x34)) <= *(float *)(kt + 0xc))
        ;
    else {
        hitv = 0;
        kt1 = 0;
    }
    if (*(int *)(ai_Unk6f0d74 + 4) == ai_Unk60cb5c && (*(int *)(kt + 0x58) == 1 || *(int *)(kt + 0x58) == 2)) {
        G19_FIGHTER_SYNC(tgt);
    }
    if (hitv != 0) {
        rnd = ai_Unk4e52f0(10);
        if (*(int *)(att + 0x16c) == ai_Unk6f0cec)
            ai_Unk4cedc0(att, hitv, 1);
        else
            ai_Unk4cedc0(att, hitv, 2);
        if (rnd > 8) {
            if (*(int *)(ev + 0x188) != *(int *)(ai_Unk6f0cec + 4))
                snd_QueueVoice(*(unsigned short *)att, ai_Unk6f0d44, "_K_C2C_SCHMERZ", -1, 500);
            else
                snd_QueueVoice(*(unsigned short *)att, ai_Unk6f0d48, "_K_C2C_SCHMERZ", -1, 500);
        } else if (rnd > 5)
            sb_PlaySample(0, "Schmerzschrei", 1, 0x3f, 0x7f, 0, 0);
    }
    if (att[8] != 0 && tgt[8] != 0 && kt1 != 0) {
        switch (*(int *)ka) {
        case 0x156:
        case 0x158:
            sb_PlaySample(hitv ? 0 : ai_Unk6f0d20, hitv ? "Treffer" : "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
            break;
        case 0x154:
        case 0x16e:
        case 0x172:
            sb_PlaySample(hitv ? 0 : ai_Unk6f0d18, hitv ? "Treffer" : "Daneben_s", 1, 0x3f, 0x7f, 0, 0);
            break;
        case 0x15e:
            sb_PlaySample(ai_Unk6f0d1c, "schuss_s", 1, 0x3f, 0x7f, 0, 0);
            break;
        case 0x160:
            if (hitv != 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            break;
        default:
            if (*(int *)ka == 0 && hitv == 0)
                sb_PlaySample(0, "Treffer", 1, 0x3f, 0x7f, 0, 0);
            break;
        }
        if (ai_Unk4d3390(att) == 0) {
            if (hitv != 0) {
                d3t_CreateBlood(*(char **)(*(char **)(att + 0x184) + 0x34), 0x40, 0.25f, 0.1f, 0.14f, 55.0f);
                if (*(char **)(*(char **)(att + 0x184) + 0x128) == 0 || (*(char **)(*(char **)(att + 0x184) + 0x128))[9] == 0x3b) {
                    ch_StopActions(*(char **)(att + 0x184));
                    a = ai_Unk4e52f0(2);
                    ai_Unk405810(*(char **)(att + 0x184), buf + a * 0x20, 1, 1.4f);
                }
                if (*(int *)ka == 0x16e) {
                    att[8] = 4;
                    G19_5176D0(buf, (char *)0x1188, *(int *)(tbl + 0x54), *(unsigned short *)att);
                    sv_SetStatusText(buf);
                }
            } else if (*(char **)(*(char **)(att + 0x184) + 0x128) == 0 ||
                       (*(char **)(*(char **)(att + 0x184) + 0x128))[9] == 0x3b) {
                ch_StopActions(*(char **)(att + 0x184));
                if (*(int *)ka == 0x15e || *(int *)ka == 0x160)
                    ai_Unk405810(*(char **)(att + 0x184), "kampf/ausweichen_seite", 1, 1.4f);
                else
                    ai_Unk405810(*(char **)(att + 0x184), "kampf/stich_abwehr", 1, 1.4f);
            }
        }
    }
}
