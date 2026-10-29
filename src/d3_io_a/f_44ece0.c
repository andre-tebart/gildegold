/* 0x44ece0, text-format line handler: reads one int token into the context's unk04 field @deep Q1/Q2: no evidence for the handler name or for AGICTX.unk04 - the binary twin bgi_Unk04 (0x44f650) reads the same field, D3BGFDESC covers offset 0x04 with pad02, and the text directive string sits in the parser table beyond 0x44ee50, outside the repo. Q6 added. */

void agi_Unk44ece0(void *unused, AGICTX *c)
{
    c->unk04 = atoi(AGI_TOK(" "));
}
