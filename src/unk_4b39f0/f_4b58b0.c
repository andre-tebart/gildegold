/* 0x4b58b0, the dialog that ends the mission chain (text 0x1b99, the same text the result dialogs 0x4b5970 /
   0x4b5c20 show as their "no more missions" option): shown by 0x4b5f10 after the result dialog when the
   mission ended outside mission mode 5, one OK form, pops on a click or a mouse move
   @deep behaviour draft is one register decision: the original keeps the literal 1 in ebx (push ebx/pop ebx in the prologue, mov ebx,1; push ebx for d2_SetForm(view,1), mov [sv_PopRequest],ebx in the loop), ours emits push 1 / mov [sv_PopRequest],1; the slots [esp+0x38]->[esp+0x3c] and the fill hunks follow from the extra ebx push. The loop body reads the same (one shared store of the || form). spell.py has no menu rule on the code hunks. Tried, one edit + one build each, reverted with the bytes unchanged: 'ok = 1; d2_SetForm(view, ok); ... sv_PopRequest = ok', 'ok = 1; d2_SetForm(view, 1); ... sv_PopRequest = ok', 'for (ok = 1; sv_RunFrame(...); ) { ... sv_PopRequest = ok; }', and the 2026-10-25 'ok = 1' local: VC6 constant-folds every one (no ebx at all). Left: the constant materialisation / register allocation (c2prio: const#0 not materialised); the only matched sibling with this idiom (ai_Unk4b3d50) gets ebx from 'r = 1', a variable with two definitions that is returned. */

void ai_MissionEndDialog(int id)
{
    int view;

    view = d2_OpenForm(0, 0, "special\\mission");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1b99);
    io_PrintText("$N");
    d2_SetForm(view, 2);
    io_PrintText((char *)0x7d);
    id |= 0x80;
    while (sv_RunFrame((void (*)(void))ai_MissionEndDialog, id)) {
        if (dm_L.rup != 0 || d2_ClickedId == 0)
            sv_PopRequest = 1;
    }
    d2_CloseView(view);
}
