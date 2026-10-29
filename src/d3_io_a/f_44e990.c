/* 0x44e990, text-format line handler: reads the current material's b8 byte from the next token */

void agi_MatB8(void *unused, AGICTX *c)
{
    AGI_M(c).b8 = atoi(AGI_TOK(" "));
}
