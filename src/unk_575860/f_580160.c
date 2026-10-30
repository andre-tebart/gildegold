/* 0x580160, the cannon's explosion spawn (1173 B, ROUGH: the branch structure and the effect calls are written,
   the matrix arithmetic is a comment): sc = the scene struct, rec = the record. The two fields sc + 0xb0 / 0xb4
   through 0x475980 and the selected object (sc + 0xa0) + 1 checked by 0x4ef090 (the character 0x770b80 +
   0x6f25b8 * 0x21c); 0x4ef260 (rec) / 0x4ef330 (rec) and the per-object table 0x5edfe0 / 0x5edfe4 / 0x5edfe8
   + the field * 0xc give the arc; three explosion records (by the selection 0, 1, 2): one (0x5dc240 / 0x5dc3a0
   matrices, 0x4eeb70 the objects, 1 record), two (two records) or three (local_68 / local_d0 / local_138, three
   records), 0x4eef50 the data; the run waited out on 0x4c3ae0 (pump 0x20007); then 0x4287b0 ("LOCATIONS_
   DOMIZIL_KANONE1_esc" 0x62a36c) and 0x429ab0 (4, the counts, the offsets, the wimpel hand sc + 0x23 / 0x24 /
   0x25) for each record and the effect 0x427050 (0, "Explosion" 0x5f36e0, the record count, 1, 0x3f, 0x7f, 0) */

void unk_Unk4ef260(char *o, float *a, float *b); float unk_Unk4ef330(); void ai_Unk580a50(); int ai_InitShot(struct SHOTREC *shot, struct SHOTPOS *pos, struct SHOTPOS *dir, float charge, float size, float density);
int ai_Unk5dc3a0(float *param_1, float param_2, float *param_3, float param_4, float *param_5, float param_6); int unk_Unk4eeb70(char *o, int *c, int *a, int *b, int *d, float v, int n, int x); int sb_PlayVariation(); void ai_Unk580a90();

void ai_Unk580160(char *sc, char *rec)
{
    int v1;
    int v2;
    int sel;
    int i;
    int h;
    int a;
    int b;
    int c;

    v1 = d2_GetObjValue(*(int *)(sc + 0xb0));
    v2 = d2_GetObjValue(*(int *)(sc + 0xb4));
    sel = (int)*(float *)(sc + 0xa0);
    h = unk_Unk4ef090((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, v1, v2, sel + 1, &a, &b);
    if (!h)
        return;
    unk_Unk4ef260(rec, &a, &b);
    c = (int)unk_Unk4ef330(rec);
    ai_Unk580a50();
    /* the per-selection explosion records: 1 (selection 0), 2 (selection 1: local_d0 + local_138) or 3
       (selection 2: local_68 + local_d0 + local_138) of 0x68-byte matrices from 0x5dc240 (the corner positions
       of the selected gun, the arc fVar4 from the table 0x5edfe0 + v1 * 0xc) and 0x5dc3a0 (the placement); the
       object list 0x770b80 + 0x6f25b8 * 0x21c through 0x4eeb70 with the 0x4eef50 value; the run waited out on
       0x4c3ae0 (pump 0x20007). The three spawns: 0x4287b0 ("LOCATIONS_DOMIZIL_KANONE1_esc"), 0x429ab0 (4, the
       offsets 0 / 0x28 / 0x28, the wimpel hand sc + 0x23 + sel) and 0x427050 (0, "Explosion", the record
       count, 1, 0x3f, 0x7f, 0) */
    /* ROUGH: the three branch bodies (the 1 / 2 / 3 explosion records through 0x5dc240 / 0x5dc3a0 / 0x4eeb70
       and the run wait 0x4c3ae0 / 0x20007) are not written yet; only the last spawn (selection 0) is below */
    {
        int u10 = (int)evt_Load((char *)(unsigned)&full_data_62a36c);
        evt_RunScriptArgs((EVTSCRIPT *)u10, 4, 0, 0, 0x28, *(int *)(sc + 0x8c + (sel + 1) * 4));
    }
    sb_PlayVariation(0, (char *)(unsigned)"Explosion", sel + 1, 1, 0x3f, 0x7f, 0);
}
