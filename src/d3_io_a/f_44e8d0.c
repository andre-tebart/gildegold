/* 0x44e8d0, text-format line handler: reads the current material's b1 byte from the next token */

void agi_MatB1(void *unused, AGICTX *c)
{
    AGI_M(c).b1 = atoi(AGI_TOK(" "));
}
