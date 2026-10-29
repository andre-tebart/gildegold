/* 0x4b2c90, the text after label k: both passes into out (text k + 1, then k + 2), then the commands of text
   k + 2 if cmd */

int hst_ParseEventText(char *out, int k, int cmd)
{
    if (!hst_ParseText_Fake_1st_Pass(out, k + 1))
        return 0;
    k += 2;
    if (!hst_ParseText_Fake_2nd_Pass(out, k))
        return 0;
    if (cmd)
        hst_ParseCommandline_2nd_Pass(k);
    return 1;
}
