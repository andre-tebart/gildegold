/* 0x4bbb00, a free animal record (counted, time stamped), 0 if all 32 are taken */

ANMREC *anm_NewAnimal(void)
{
    int i;

    for (i = 0; i < 32; i++) {
        if (anm_Animals[i].obj == 0)
            break;
    }
    if (i == 32)
        return 0;
    anm_NAnimals++;
    anm_Animals[i].birth = d3_Time;
    return &anm_Animals[i];
}
