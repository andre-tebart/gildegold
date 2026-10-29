/* 0x44ea90, text-format line handler: reads the current material's f[3] float from the next token */

void agi_MatF3(void *unused, AGICTX *c)
{
    AGI_M(c).f[3] = atof(AGI_TOK(" "));
}
