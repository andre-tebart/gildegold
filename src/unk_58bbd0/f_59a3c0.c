/* 0x59a3c0, the town map screen (416 B): the three city points 0x7e4234 + i * 0x2f4 (0x59a2c0, named
   "stadt_%s" over the object name at +0, registered by 0x52e1c0); the frame pump 0x5309a0 (0x59a3c0,
   0x677d7) refreshes the contacts, asks 0x448630 ((float)(short)0x1467380, (float)(short)0x1467382, 0x60, 1)
   and opens the info window 0x599ef0 (record, 1) of the character in that city (0x501190 (1, 0, 0x47), the
   slot +0x65); on exit the three objects are freed (0x447190) between 0x52e190 / 0x52e170, then 0x52d7a0 */

void ai_Unk59a3c0(void)
{
    int objs[4];
    char *city;
    int i;
    int rec;
    int r;

    objs[0] = 0;
    objs[1] = 0;
    objs[2] = 0;
    objs[3] = 0;
    for (city = (char *)((unsigned)&ai_Unk7e422c + 0x8), i = 0; city < (char *)((unsigned)((unsigned)ai_Unk7e422c + 0x5c) + 0x888); city += 0x2f4, i++) {
        objs[i] = ai_Unk59a2c0(city);
        sprintf((char *)objs[i], "stadt_%s", city);
        sv_RegisterNamePlate(objs[i], city, 0);
    }
    d2_SetPointerSprite(0, 0);
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_Unk59a3c0, 0x677d7) != 0) {
        U_I((unsigned)&ai_ClickedObject) = 0;
        for (city = (char *)((unsigned)&ai_Unk7e422c + 0x8), i = 0; city < (char *)((unsigned)((unsigned)ai_Unk7e422c + 0x5c) + 0x888); city += 0x2f4, i++)
            sv_RegisterNamePlate(objs[i], city, 0);
        r = d3if_Pick((float)(short)U_I((unsigned)&dm_L), (float)(short)U_I(((unsigned)&dm_L + 0x2)), 0x60, 1);
        U_I((unsigned)&ai_Unk6f22e8) = r;
        if (U_I(((unsigned)&dm_L + 0xc)) == 0 || r == 0) {
            if (U_I(((unsigned)&dm_L + 0x20)) != 0)
                U_I((unsigned)&sv_PopRequest) = 1;
        } else {
            U_I((unsigned)&ai_ClickedObject) = r;
            for (i = 0; i < 3; i++) {
                if (U_I((unsigned)&ai_ClickedObject) == objs[i] && (rec = gm_BuildQuery(1, 0, 0x47)) != 0) {
                    do {
                        if (U_I(rec + 0x65) == i + 1) {
                            ai_Unk599ef0((char *)rec, 1);
                            break;
                        }
                        rec = gm_BuildNext();
                    } while (rec != 0);
                }
            }
        }
    }
    sv_ReleaseNamePlates();
    for (i = 0; i < 3; i++)
        if (objs[i] != 0)
            d3if_Delete((struct D3OBJECT *)objs[i]);
    sv_ClearNamePlates();
    sv_CameraFollow();
}
