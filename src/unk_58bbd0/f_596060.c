/* 0x596060, the idle contact listener (28 B): 0x52e190, then the pump 0x5309a0 (this, 0x67fff) until 0 */

void ai_IdleContact(int building, int room)
{
    int r;

    sv_ReleaseNamePlates();
    do {
        r = sv_RunFrame(ai_IdleContact, 0x67fff);
    } while (r != 0);
}
