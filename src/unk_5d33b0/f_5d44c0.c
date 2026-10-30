/* 0x5d44c0, an edit field dialog (261 B): opens an input at (x, y) with the font height, font 0x53,
   the text and the focus; the width / max width of its D2INPUT become 0x80 / 0x110 and its maximum
   value 0x1f. The pump runs with this function; Enter (kb_Last 0x1c) copies the edited text
   (d2_GetObjValue) into text, the mouse event or a lost focus (d2_FocusedInput == 0) only closes the
   dialog; sv_PopRequest asks MASTER to pop the screen. d2_RemoveObj destroys the control at the end.
   Returns 1 when the text was taken over, else 0. */


int ai_EditText(int x, int y, char *text)
{
    int id;
    int ret;
    char *src;
    char *dst;
    int *input;

    ret = 0;
    id = d2_OpenInput(x, y, 0, d2_FontHeight, 1);
    d2_SetObjFont(id, 0x53);
    d2_SetObjValue(id, text);  /* @original: this call site pushes 2 arguments (0x5d4500) against the 5-parameter definition */
    d2_FocusInput(id);
    input = &d2_ObjsV[id].input;
    d2_Inputs[*input].w = 0x80;
    d2_Inputs[*input].maxw = 0x110;
    d2_SetObjMax(id, 0x1f);
    while (sv_RunFrame((int)ai_EditText, sv_FrameMask) != 0) {
        if (kb_Last == 0x1c) {
            src = (char *)d2_GetObjValue(id);
            dst = text;
            while ((*dst++ = *src++) != '\0')
                ;
            sv_PopRequest = 1;
            ret = 1;
        }
        if (dm_L.rup != 0 || d2_FocusedInput == 0)
            sv_PopRequest = 1;
    }
    d2_RemoveObj(id);
    return ret;
}
