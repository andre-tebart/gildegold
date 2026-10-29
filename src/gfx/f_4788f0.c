/* 0x4788f0, removes hit rectangle i */

void d2_RemoveHitRect(int i)
{
    if (d2_HitCount >= 1 && i >= 0) {
        memset(&d2_HitRects[i], 0, sizeof(D2HIT));
        d2_HitCount--;
    }
}
