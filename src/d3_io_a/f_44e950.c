/* 0x44e950, text-format line handler: reads the current material's b4 byte from the next token */

void agi_MatB4(void *unused, AGICTX *c)
{
    AGI_M(c).b4 = atoi(AGI_TOK(" "));
}
