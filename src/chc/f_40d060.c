/* 0x40d060, SetCharacterStepSample(name): sets the step sound sample name (ch_StepSample); always 1 */

int chc_SetCharacterStepSample(char **name)
{
    strcpy(ch_StepSample, *name);
    return 1;
}
