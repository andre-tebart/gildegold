/* 0x42bd60 */

int sfx_ResumeSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_resume_sample(s);
    } else
        ret = -1;
    return ret;
}
