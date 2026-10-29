/* 0x47dd00, the AI's work limit for a frame, from the smoothed game rating: rating * 0.006, clamped to 5..16
   (the callers compare the current load ai_WorkLoad against it) */

int ai_WorkLimit(void)
{
    int n;

    n = (int)(ai_Rating * 0.006);
    if (n < 5)
        return 5;
    if (n > 0x10)
        n = 0x10;
    return n;
}
