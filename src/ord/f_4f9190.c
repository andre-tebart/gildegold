/* 0x4f9190, sends the message 0x4c5e80 built from the record 0x91b480 with its fields replaced by
   (a, 0, ai_Time, b, c, 0, d, 0, 0) and the tag 'init' at +0x24: the first frame of a game */

void ord_InitMessageRecord(int a, float b, float c, float d)
{
    ORDINITMSG m;

    ORD_COPY(&m, offset ord_MessageRecord, 0x24);
    m.tag = 0x696e6974;
    m.record.enabled = a;
    m.record.count = 0;
    m.record.time = ai_TimeYear;
    m.record.base = b;
    m.record.scale = c;
    m.record.value = 0.0f;
    m.record.limit = d;
    m.record.phase = 0;
    m.record.stage = 0;
    cm_SendMessage56(&m);
}
