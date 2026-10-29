/* 0x44ea10, text-format line handler: reads the current material's f[1] float from the next token */

void agi_MatF1(void *unused, AGICTX *c)
{
    AGI_M(c).f[1] = atof(AGI_TOK(" "));
}
