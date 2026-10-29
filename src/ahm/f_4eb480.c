/* 0x4eb480, loads the prototype tables at start: with mode 0 from "/gamedata/ai/alchemy.dfn" (ahm_LoadDfn), else from "alchemy_data.ini" and then written back
   to the .dfn (ahm_SaveDfn); every load restarts the walk (ahm_ShufflePlayers); 0 when the ini load failed, else 1 */

int ahm_LoadPrototypes(int mode)
{
    if (mode != 0) {
        if (ahm_LoadIni() != 0) {
            ahm_ShufflePlayers(1);
            return ahm_SaveDfn();
        }
        return 0;
    }
    if (ahm_LoadDfn() != 0)
        ahm_ShufflePlayers(1);
    return 1;
}
