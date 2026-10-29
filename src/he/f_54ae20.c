/* 0x54ae20, a Ghidra miss: the start handler: the due clock +0x52 = now, posted 2 units ahead */

int ls_GetLanguage();
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);

void he_Unk54ae20(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 2, 0);
}
