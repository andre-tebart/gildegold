/* 0x40c9a0, SetCharacterCamera(ch, view): cuts to the camera preset named CLOSEUP, LEFT_SHOULDER, RIGHT_SHOULDER or EGO (0 .. 3); 1 after an error, else 0 */

int chc_SetCharacterCamera(CHARACTER **ch, char **view)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetCharacterCamera(): Invalid character");
        return 1;
    }
    if (!strcmp("CLOSEUP", *view))
        ch_SetCamera(*ch, 0);
    else if (!strcmp("LEFT_SHOULDER", *view))
        ch_SetCamera(*ch, 1);
    else if (!strcmp("RIGHT_SHOULDER", *view))
        ch_SetCamera(*ch, 2);
    else if (!strcmp("EGO", *view))
        ch_SetCamera(*ch, 3);
    return 0;
}
