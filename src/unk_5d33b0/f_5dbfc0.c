/* 0x5dbfc0, writes the wind state to the save game (241 B; the save writer 0x51c870 calls it): the fields of
   ai_EffectState one by one with vfs_fwrite, the clock of the last update, the sin / cos / base / gust /
   phase tables and the angle. 0 as soon as a write fails, else 1. */

int ai_SaveWind(struct VFSFILE *f)
{
    if (vfs_fwrite(&ai_EffectState.year, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_EffectState.hour, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(&ai_EffectState.min, 4, 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.sinTab, sizeof(ai_EffectState.sinTab), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.cosTab, sizeof(ai_EffectState.cosTab), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.base, sizeof(ai_EffectState.base), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.gust, sizeof(ai_EffectState.gust), 1, f) <= 0)
        return 0;
    if (vfs_fwrite(ai_EffectState.phase, sizeof(ai_EffectState.phase), 1, f) <= 0)
        return 0;
    return vfs_fwrite(&ai_EffectState.angle, 4, 1, f) > 0;
}
