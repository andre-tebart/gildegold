/* 0x4eac60, fills the three steps ai_WalkSteps from the shuffled table ai_PlayerPrototypes (the entries in turn) with the running positions (x, y) on a 25 x 25 grid;
   when the table has been walked through once it is shuffled again (ahm_ShufflePlayers)
   @deep B3 draft: 54 code hunks, all register scheduling - the table pointer ai_PlayerPrototypes is hoisted into ebp before the do-loop with the entry fields read via [ebp+eax*4+N]; the original reloads [0x6f0e2c] each iteration and reads the fields through the entry pointer in eax; our s++ is scheduled at the loop top ([ecx-0x1c] stores) while the original does lea edx,[ecx+4] then add ecx,0x18. Every statement, constant, field offset, the 3-step bound and the trailing ahm_ShufflePlayers(1) match. Last-bytes leads tried: loop end as ai_WalkSteps+3 (54 -> 72 code hunks, WORSE, reverted), as (int)&ai_PlayerPrototypeCount (same 54, kept for Q3), the trailing compare from the index side ai_WalkIndex >= count (62 -> 54, kept), the menu reload lead (drop the g local, re-read ai_PlayerPrototypes[i] at the use, same 54), the dup-tail lead (branch polarity, kept). Q1..Q7 pass. Left: allocation/scheduling of the table and step pointers. */

void ahm_FillSteps(void)
{
    AHMSTEP *s;
    int i;

    if (ai_PlayerPrototypeCount != 0) {
        s = ai_WalkSteps;
        do {
            i = ai_WalkIndex % ai_PlayerPrototypeCount;
            ai_WalkIndex++;
            s->grid.w[0] = ai_PlayerPrototypes[i].w[0];
            s->grid.w[1] = ai_PlayerPrototypes[i].w[1];
            s->grid.w[2] = ai_PlayerPrototypes[i].w[2];
            s->y = ai_WalkY;
            s->x = ai_WalkX;
            s->on = 1;
            ai_WalkY = (ai_WalkDY + ai_WalkY) % 0x19;
            ai_WalkX = (ai_WalkDX + ai_WalkX) % 0x19;
            s++;
        } while ((int)s < (int)((unsigned)ai_WalkSteps + 0x48));
        if (ai_WalkIndex >= ai_PlayerPrototypeCount)
            ahm_ShufflePlayers(1);
    }
}
