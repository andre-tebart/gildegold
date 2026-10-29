/* 0x47e940, resets the job state of a working character (kinds 1 / 2 with a job): clears the 16 bytes
   at 0x1b4 (the flags, budget, product and target) and sets the job flag bit 0 */

void ai_ResetJob(AICHAR *c)
{
    unsigned int *f;

    if ((c->kind == 1 || c->kind == 2) && c->jobId) {
        memset(&c->flags1b4, 0, 16);
        c->target = -1;
        c->flags1b4 |= 1;
    }
}
