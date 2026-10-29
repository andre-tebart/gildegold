/* 0x4de5e0, fills the 0x92bcc0 table (0x505ca0) for n entries */

void ai_Unk4de5e0(int n)
{
    gm_Shuffled(n, ai_Unk92bcc0);
    ai_Unk60d370 = n;
    ai_Unk6f0db0 = 0;
}
