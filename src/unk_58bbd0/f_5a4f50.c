/* 0x5a4f50, an empty listener (28 B, room 0x4d of the building driver 0x59e420): 0x52e190 and the pump
   0x5309a0 (this, 0x67fff) until it returns 0 */

void ai_ThievesGuildEmptyListener(void)
{
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_ThievesGuildEmptyListener, 0x67fff) != 0)
        ;
}
