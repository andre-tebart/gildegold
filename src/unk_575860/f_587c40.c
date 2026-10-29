/* 0x587c40, ends the character ch's extra -- the "sp_<id>" role pose script that sv.c 0x52b490 / 0x52b500
   links back at the model +0x28 (the model at +0x184) / +0xc94 -- when one runs (0x4284a0 evt_FindById,
   0x429e90 evt_Stop), then toggles the at-work flag bit 0 of +0x1b4 by f's bit 0 (f = 0: ends the pose only,
   the form the AI (ai_EndWorkerPose) and the graveyard "Schlag" action (0x5867c0, then 0x52b490 with the new
   pose) use) */


void ai_EndCharExtra(char *ch, int f)
{
    int h;

    if (*(int *)(*(int *)(ch + 0x184) + 0x28) != -1) {
        h = (int)evt_FindById(*(int *)(*(int *)(ch + 0x184) + 0x28));
        if (h != 0)
            evt_Stop((EVTSCRIPT *)h);
    }
    *(int *)(ch + 0x1b4) ^= f & 1;
}
