/* 0x40bbb0, KillCharacter(ch): deletes the character and returns ch_Delete's result. */

int chc_KillCharacter(CHARACTER **ch)
{
    return ch_Delete(*ch);
}
