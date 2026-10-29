/* 0x42bbf0 */

int sfx_SetSampleFile(HSAMPLE s, char *type, void *image, int size)
{
    int ret = 0;

    if (!sfx_Initialized || sfx_FindSample(s) < 0 || !AIL_set_named_sample_file(s, type, image, size, ret))
        ret = -1;
    return ret;
}
