/* 0x40ca60, MoveCharacterCamera(ch, view, time): moves over time to the camera preset named CLOSEUP, LEFT_SHOULDER, RIGHT_SHOULDER or EGO (0 .. 3); 1 after an error, else 0 */

int chc_MoveCharacterCamera(CHARACTER **ch, char **view, int *time)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "MoveCharacterCamera(): Invalid character");
        return 1;
    }
    if (!strcmp("CLOSEUP", *view))
        ch_MoveCamera(*ch, 0, *time);
    else if (!strcmp("LEFT_SHOULDER", *view))
        ch_MoveCamera(*ch, 1, *time);
    else if (!strcmp("RIGHT_SHOULDER", *view))
        ch_MoveCamera(*ch, 2, *time);
    else if (!strcmp("EGO", *view))
        ch_MoveCamera(*ch, 3, *time);
    return 0;
}
