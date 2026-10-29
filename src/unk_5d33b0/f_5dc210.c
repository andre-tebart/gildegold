/* 0x5dc210, the two-axis position share (35 B): the two values a / b divided by the sphere volume
   record's reach (+0x10, volume * radius - ai_SphereVolume's record, am.c 0x4a67a0 passes it as
   int *in) and scaled by 0.1; the pair goes into out (+0, +4) as the ballistic range rng[0] / rng[1].
   Void: the caller ignores the result (am.h declares it void). The division is written value / field
   (no parentheses): the parenthesised form makes VC6 emit fld divisor; fdivr value instead. */

void ai_PositionShare(float a, float b, SPHEREVOLUME *rec, float *out)
{
    out[0] = a / rec->reach * 0.1f;
    out[1] = b / rec->reach * 0.1f;
}
