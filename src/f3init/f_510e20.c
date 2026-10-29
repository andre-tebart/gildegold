/* 0x510e20, prints the title ("$C") and the text t in the form 0x75df00 */


void init_ShowStatus(int t)
{
    d2_SetForm(init_StatusForm, 2);
    io_PrintText("$C");
    io_PrintText(t);
    d2_DrawAll();
}
