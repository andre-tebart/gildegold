/* 0x4ec2f0, clears the step p (its flag) */

void ahm_ClearStep(AHMSTEP *p)
{
    int i;

    i = 0;
    do {
        if (&ai_WalkSteps[i] == p) {
            ai_WalkSteps[i].on = 0;
            return;
        }
        i++;
    } while (i < 3);
}
