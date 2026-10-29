/* 0x4ec2d0, the step number i (0 .. 2) of 0x6f0de0; 0 for others */


AHMSTEP *ahm_WalkStep(int i)
{
    if (i >= 0 && i < 3)
        return &ai_WalkSteps[i];
    return 0;
}
