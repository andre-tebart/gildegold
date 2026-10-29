/* 0x512780, the scene object callback for "gate" objects: sets the state (+0x224 = 1, the flag 1 of +0x211 cleared) when the name contains "gate", else the state 0 */


void init_GateObjectHook(int o)
{
    if (strstr((char *)o, "gate") != 0) {
        *(int *)(o + 0x224) = 1;
        *(unsigned char *)(o + 0x211) &= 0xfe;
        return;
    }
    *(int *)(o + 0x224) = 0;
}
