/* 0x4c1fc0, the location track of building kind kind, -1 if none @deep B draft, 24 code-hunk instructions, no meaning error: read the original 0x4c1fc0..0x4c1fff against the source - the table walk (kinds[], kind compare, i++), the end test (l->kinds[0]) and the -1 return all correspond; only register allocation / scheduling differs. Ours keeps l->kinds[0] in esi from the loop entry (0x4c1fce) and tests esi after advancing edi; the original reloads kinds[0] into ecx (mov ecx,[esi] at 0x4c1fe6), advances both esi (l) and edi (a pointer it increments but never reads) and tests ecx. */

int snd_FindTrackKind(int kind)
{
    int i;
    int *k;
    G16LOC *l;

    i = 0;
    l = snd_LocationTracks;
    do {
        for (k = l->kinds; *k != 0; k++) {
            if (*k == kind)
                return i;
        }
        i++;
    } while ((l++)->kinds[0] != 0);
    return -1;
}
