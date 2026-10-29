/* 0x545640, frees the event record (and its job record 0x4bb6c0 at +0xac unless the state is -2) */

int ai_Unk4bb6c0();

void he_Unk545640(int ev)
{
    if (*(int *)(ev + 0x70) != -2)
        ai_Unk4bb6c0(ev + 0xac);
    he_Unk53e1f0((int *)ev);
}
