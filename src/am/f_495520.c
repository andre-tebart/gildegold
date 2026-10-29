/* 0x495520, registers an action: its need weights come from the ini file when ini is set */

int ai_Unk495520(AIACT *a, int ini)
{
    G5ACT *act = (G5ACT *)a;
    char key[64];
    char val[128];
    int i, found;

    if (act->id != 0 && act->id < 0x49) {
        if (ini == 0) {
            for (i = 0; i < 4; i++) {
                act->w1[i].k = 0xff;
                act->w1[i].w = 0.0f;
                act->w2[i].k = 0xff;
                act->w2[i].w = 0.0f;
                act->w3[i].k = 0xff;
                act->w3[i].w = 0.0f;
            }
        } else {
            for (i = 1; i <= 4; i++) {
                sprintf(key, "ShortDesire%li", i);
                if (GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0) == 0) {
                    act->w1[i - 1].k = 0xff;
                    act->w1[i - 1].w = 0.0f;
                } else {
                    act->w1[i - 1].k = (unsigned char)ai_Unk4a47c0(val);
                    sprintf(key, "ShortChange%li", i);
                    GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0);
                    act->w1[i - 1].w = (float)atof(val);
                }
                sprintf(key, "LongDesire%li", i);
                if (GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0) == 0) {
                    act->w2[i - 1].k = 0xff;
                    act->w2[i - 1].w = 0.0f;
                } else {
                    act->w2[i - 1].k = (unsigned char)ai_Unk4a47c0(val);
                    sprintf(key, "LongChange%li", i);
                    GetPrivateProfileStringA(act->name, key, "", val, 0x80, ai_Unkf83da0);
                    act->w2[i - 1].w = (float)atof(val);
                }
            }
            found = 0;
            for (i = 0; !found; i++) {
                if (i > 3) {
                    sprintf(val, "am_RegisterMethod(): Methode '%s' hat keinerlei Auswirkungen!!!", act->name);
                    break;
                }
                if ((act->w1[i].k != 0xff && act->w1[i].w != 0.0f) ||
                    (act->w2[i].k != 0xff && act->w2[i].w != 0.0f))
                    found = 1;
            }
            memcpy(act->w3, act->w1, 0x20);
        }
        G5_ACTS[act->id] = *act;
    }
    return 1;
}
