/* 0x44ea50, text-format line handler: reads the current material's f[2] float from the next token */

void agi_MatF2(void *unused, AGICTX *c)
{
    AGI_M(c).f[2] = atof(AGI_TOK(" "));
}
