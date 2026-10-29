/* 0x521ba0, draws the mouse pointer for the panel layout d2_PointerLook: the scroll arrows 3 / 6 show it at (-0x28, 0),
   8 at (-0x28, -0x30), the bar layouts 1 / 7 at (0, -0x30) and every other layout at (0, 0)
   @deep behaviour draft, readable done. 65 -> 19 code hunks this session: the width was the whole first half (d2_PointerLook is a short at 0x6ed9d0, the original loads a word and cmp ax; the int declaration made dword compares) and the shape is one if/else-if chain in the original's test order 1 / 3 / 6 / 8 / 7 with x = -0x28 in the 3 / 6 / 8 arms and y = -0x30 in 1 / 7, each of 3 / 6 / 8 calling and returning. Tried, all 19 code hunks: x, y locals assigned at the top; the same with declaration initializers; 'x = y = 0'; literal args in the arms (-0x28, y) / (-0x28, -0x30) / (0, y); an explicit 'x = 0' in the 1-arm; a switch over the 5 cases (a jump table, 77 hunks). Left: the original keeps BOTH x = 0 (edx) and y = 0 (ecx) live from the entry (xor edx,edx / xor ecx,ecx) and pushes them (mov edx,-0x28; push ecx; push edx), where VC6 folds ours to 'push 0 / push -0x28' and drops x's register - a constant-fold / peephole choice (spell: 521ba6 dup-tail, the 4-instruction 3-arm tail, hints tried as listed). vargen / perm is what is left. */

void sv_DrawPanelPointer(void)
{
    int x, y;

    x = 0;
    y = 0;
    if (d2_PointerLook == 1)
        y = -0x30;
    else if (d2_PointerLook == 3) {
        x = -0x28;
        d2_DrawMousePointer(x, y);
        return;
    } else if (d2_PointerLook == 6) {
        x = -0x28;
        d2_DrawMousePointer(x, y);
        return;
    } else if (d2_PointerLook == 8) {
        x = -0x28;
        y = -0x30;
        d2_DrawMousePointer(x, y);
        return;
    } else if (d2_PointerLook == 7)
        y = -0x30;
    d2_DrawMousePointer(x, y);
}
