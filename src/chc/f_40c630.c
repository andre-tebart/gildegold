/* 0x40c630, GiveObject(ch, name): puts the named object into the character's right hand (ch_SetHandObject); 0 when the character is invalid, else 1 */

int chc_GiveObject(CHARACTER **ch, char **name)
{
    if (!*ch)
        return 0;
    ch_SetHandObject(*ch, 2, *name);
    return 1;
}
