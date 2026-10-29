/* 0x53ed40, runs 0x53ec40 on every record of the player *pl (type 2 filter) */

void he_Unk53ed40(unsigned short *pl)
{
    int iv = (int)he_FindEvent(1, 2, *pl);

    while (iv != 0) {
        he_Unk53ec40((unsigned char *)iv);
        iv = (int)he_NextEvent();
    }
}
