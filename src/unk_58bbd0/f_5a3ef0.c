/* 0x5a3ef0, an empty listener (28 B): the pump 0x5309a0 (this, 0x67fff) until it returns 0 */

void ai_DungeonEmptyListener(void)
{
    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_DungeonEmptyListener, 0x67fff) != 0)
        ;
}
