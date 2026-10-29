/* 0x4942a0, a level from the score: score / 4, clamped to 2..6 */

int ai_Unk4942a0(AIPLAN *q)
{
    return (int)min(max(q->unk04 * 0.25f, 2.0f), 6.0f);
}
