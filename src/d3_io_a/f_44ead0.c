/* 0x44ead0, text-format line handler: reads the current material's f[4] float from the next token */

void agi_MatF4(void *unused, AGICTX *c)
{
    AGI_M(c).f[4] = atof(AGI_TOK(" "));
}
