/* 0x40c4f0, TakeObjectScript(ch, name, n, obj, frame, script): takes the object with the named
   animation at frame, stores the object's model name in the action (name2) and runs script through
   chc_AniScriptCB; 1 after an error, else 0
   @deep Q4 D3MATERIAL_NAME104 (0x104 of D3MATERIAL read as the model-name char*): the typed union name/ops in d3.h flips d3_MirDrawObject by 8 bytes at +0x1be (build REGRESSED, the known type debt of notes/vc6-matching.md); no other typed access possible without that header change */

int chc_TakeObjectScript(CHARACTER **ch, char **name, int *n, D3OBJECT **obj, int *frame, char **script)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_TakeObjectScript) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_TakeObjectScript;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectScript(): Invalid character");
        return 1;
    }
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectScript(): Invalid dummy");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniScriptCB, *frame, 0x31, *n);
    if (a) {
        strcpy(a->u.name, *name);
        if (*obj && (*obj)->material && D3MATERIAL_NAME104((*obj)->material))
            strcpy(CH_ACTOBJ(a)->name2, D3MATERIAL_NAME104((*obj)->material));
        strcpy(a->scriptName, *script);
        a->scriptAction.group = evt_NoGroup;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_TakeObjectScript;
    return 0;
}
