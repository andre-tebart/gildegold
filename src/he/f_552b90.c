/* 0x552b90, the run handler of an event driven by 0x578120: when it returns nonzero the record is freed, else the state is 0 */

int ai_Unk578120();
int ai_Unk577740();
int ai_Unk575860();
int ai_Unk577820();
void ai_Unk578150(void);
void sv_PopTo(int v);
int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay);

void he_Unk552b90(int ev)
{
    if (ai_Unk578120(ev) != 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(int *)(ev + 0x70) = 0;
}
