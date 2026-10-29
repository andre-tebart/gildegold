/* 0x403ab0, 1 when the current animation has reached its last frame (last, or nframes when last is -1),
   or when bit 5 of its flags byte is set; 1 also when the character has neither a slot nor a next anim
   @deep behaviour: close (8 differing asm lines / 14 bytes), only the zero-extension of the last return: original xor ecx,ecx; mov cl,[eax+0x6d]; mov eax,ecx; shr eax,5; and eax,1, ours mov al,[eax+0x6d]; shr eax,5; and eax,1 (same value, the upper bits are dead before the shift). spell.py menu: only dup-tail 403af2, an artifact of the 3 extra original instructions - the if/else and both early returns match instruction for instruction (23 equal), so the dup-tail prescriptions (statements in both arms / pull after the branch / swap the branch polarity) do not apply. Width spellings tried, one edit + one build each, all identical: unsigned int local, unsigned short local, (unsigned int) cast, (unsigned short) cast, b[1] / 32, ((unsigned)b[1] >> 5) % 2, unsigned char bitfield view flags6d : 8, (b[1] & 0x20) ? 1 : 0. Earlier session's list (notes/units/ch.md): (unsigned)/(int)/(unsigned long) casts, (b & 0x20) >> 5, a local, byte and int bitfield overlays, a ternary. VC6 canonicalises every spelling to the trimmed AL load; the original kept the zero-extended byte. Q6 description added, Q1/Q2/Q3/Q4/Q5/Q7 pass. */

int ch_AnimDone(CHARACTER *ch)
{
    D3ANIMSLOT *s = ch->slot;
    int n;

    if (!ch->slot && !ch->next)
        return 1;
    if (!s)
        return 0;
    n = s->data->last != -1 ? s->data->last : s->data->nframes;
    if (s->frame >= n)
        return 1;
    return s->flags.b[1] >> 5 & 1;
}
