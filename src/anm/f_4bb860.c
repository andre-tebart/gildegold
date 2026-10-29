/* 0x4bb860, the animal step: every 350 ms maybe a new animal (not in season 3), then one of the 32 records is
   updated by kind and removed when flagged
   @deep Q2 + Q4: ai_GfxDetail (0x6f35c4, no writer in src; the step reads it as the animal limit 0/16/32 - name unproven) and 3 raw accesses obj+0x34->+0x1ec->+0x96d flag 0x40 (D3MATERIAL has no field at 0x96d) */

/* d3_Time from which the record may be flagged for removal (remove): 2100 ms after its birth time */
#define ANM_AGE_TIME(rec) ((rec).birth + 0x834)

void anm_Update(void)
{
    int scene, s, m;

    scene = d3_CurrentScene;
    s = gm_ClockSeason(ai_Time);
    d3if_SelectScene(0, 1);
    if (ai_GfxDetail == 0)
        m = 0;
    else
        m = ai_GfxDetail == 1 ? 0x10 : 0x20;
    if (d3_Time > anm_LastSpawn + 0x15e) {
        ANMREC *a;

        if ((int)(util_Random(m * 2) - m) > anm_NAnimals && s != 3 && (a = anm_NewAnimal()) != 0) {
            switch (util_Random(100) > 0x1e ? util_Random(5) + 3 : util_Random(2)) {
            case 1:
                anm_CreateCat(a);
                break;
            case 0:
                anm_CreateDog(a);
                break;
            case 4:
                anm_CreateSheep(a);
                break;
            case 3:
                anm_CreateCow(a);
                break;
            case 6:
                anm_CreateAnimal(a, 6);
                break;
            case 7:
                anm_CreateAnimal(a, 7);
                break;
            }
        }
        anm_LastSpawn = d3_Time;
    }
    if (anm_Animals[anm_NextAnimal].obj != 0) {
        switch (anm_Animals[anm_NextAnimal].kind) {
        case 0:
        case 1:
            anm_UpdateCat(&anm_Animals[anm_NextAnimal]);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            anm_UpdateSheep(&anm_Animals[anm_NextAnimal]);
            break;
        case 2:
            anm_UpdateBird(&anm_Animals[anm_NextAnimal]);
            break;
        }
        if (s == 3)
            anm_Animals[anm_NextAnimal].remove = 1;
        /* the expression written out each time, no local for it or for the record: VC6 then reloads unk20 and
           recomputes the record address inside the block, as the original does */
        if (d3_Time > ANM_AGE_TIME(anm_Animals[anm_NextAnimal])) {
            if (util_RandomF() > ANM_AGE_TIME(anm_Animals[anm_NextAnimal])
                                     / (d3_Time - ANM_AGE_TIME(anm_Animals[anm_NextAnimal])))
                anm_Animals[anm_NextAnimal].remove = 1;
        }
        if (anm_Animals[anm_NextAnimal].remove
            && ((*(unsigned char *)(*(char **)(*(char **)((char *)anm_Animals[anm_NextAnimal].obj + 0x34) + 0x1ec)
                                    + 0x96d) & 0x40)
                || s == 3 || anm_NAnimals > m))
            anm_RemoveAnimal(&anm_Animals[anm_NextAnimal]);
    }
    d3if_SelectScene(scene, 1);
    anm_NextAnimal = (anm_NextAnimal + 1) % 32;
}
