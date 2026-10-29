/* 0x5dbc20, the default effect scale (13 B): the scale field of the default effect record
   (ai_DefaultEffectScaleBits 0x62de6c) through the 0 .. 60 -> 0 .. 1 clamp 0x5dd090
   (ai_NormalizeEffectScale); the result is the entry's scale (passed to the 0 .. 100 slider as
   100 * value, unk_575860.c). The callers pass an ignored argument through a K&R declaration.
   @original the original TU called 0x5dd090 through an int K&R declaration: it pushes the float's
   bit pattern as an int (mov eax,ds:0x62de6c; push eax) and does not pop the float result, so the
   function returns the callee's st(0) (float) and the result is used as such by its callers. */

float ai_GetDefaultEffectScale(void)
{
    ((int (__cdecl *)())ai_NormalizeEffectScale)(ai_DefaultEffectScaleBits); /* @original the float callee was called through an int K&R declaration (push the bits, no fstp) */
}
