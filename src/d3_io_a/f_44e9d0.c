/* 0x44e9d0, text-format line handler: reads the current material's f[0] float from the next token */

void agi_MatF0(void *unused, AGICTX *c)
{
    AGI_M(c).f[0] = atof(AGI_TOK(" "));
}
