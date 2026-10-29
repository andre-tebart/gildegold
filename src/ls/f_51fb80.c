/* 0x51fb80, the save game's reader of the avatar table: forwards to gv_ReadAvatars (0x4ed270) */


int ls_ReadAvatars(void *f)
{
    return gv_ReadAvatars(f);
}
