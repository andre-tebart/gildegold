/* 0x5740d0, the run of the event 0x9f: -2 / -1 free */

void he_Unk5740d0(int ev)
{
    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2)
        he_Unk53e1f0((int *)ev);
}
