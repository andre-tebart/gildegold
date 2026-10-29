/* 0x4b3ca0, gives owner a mission of type (in a free slot; game mode 5: always slot 0) @deep Q2 only, byte: the store order of the mode-5 block and of the loop was the behaviour gap (type before owner in BOTH blocks - the original loads type into eax first and stores it first; the loop draft had owner first, 10 -> 4 differing instructions, then the mode-5 block back to type first gave the byte match, check 2567 -> 2568). The only open rule is G13MIS.unk20 (+0x20, a byte zeroed at creation / restart and saved / read back by 0x4b62e0 / 0x4b63b0, never read otherwise in any drafted function): no evidence for a name, so the field stays unk and Q2 cannot clear. Q1 done (ai_NewMission -> ai_NewMission: gives the owner a mission of the type in a free slot). */

G13MIS *ai_NewMission(int owner, int type, int x)
{
    int i;

    if (ai_MissionMode == 5) {
        ai_Missions[0].type = type;
        ai_Missions[0].owner = owner;
        ai_Missions[0].time.year = 0;
        gm_SetTime(&ai_Missions[0].time, 0, 0, 0);
        ai_Missions[0].unk20 = 0;
        return &ai_Missions[0];
    }
    for (i = 0; i < 0x80; i++) {
        if (ai_Missions[i].type == 0) {
            ai_Missions[i].type = type;
            ai_Missions[i].owner = owner;
            ai_Missions[i].time.year = 0;
            gm_SetTime(&ai_Missions[i].time, 0, 0, 0);
            ai_Missions[i].unk20 = 0;
            return &ai_Missions[i];
        }
    }
    return 0;
}
