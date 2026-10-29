/* 0x427010 */

void sb_Resume(SBSAMPLE *s)
{
    if (sb_Driver && s->handle) {
        if (!s->info->data)
            sb_Play(s, 0, 0);
        else
            sfx_ResumeSample(s->handle);
    }
}
