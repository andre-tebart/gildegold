/* 0x55a030, the run handler of the espionage event ("Spionage: next target = %G" debug text, 5 states by the jump table 0x55a5d0; ROUGH skeleton): the spy's job handles (0x4c3ae0 / 0x4c3b10), the player record, the next target (0x524770 walk checks, 0x524260 group release, 0x4c52e0 / 0x4c5230 "Spionage" send, 0x4c4c60 posts, 0x4c4e00 stops) and the end frees the record */

void he_Unk55a030(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p, h;

    if (st < -2 || st > 2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0 || st < 0) {
        if (p != 0) {
            ai_Unk4c5230(*(int *)(p + 4));
            ((void (*)(int))cm_SendKillCharacter)(*(int *)(p + 4));
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    h = *(int *)(ev + 0xc4);
    if (h != -1 && cm_GetSlotState(h) == 0)
        return;
    ai_Unk4c5230(*(int *)(p + 4));
    ai_Unk4c52e0(*(int *)(p + 4), *(int *)(ev + 0x10), -1, 0, 0, "Spionage");
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
}
