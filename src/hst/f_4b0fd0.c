/* 0x4b0fd0, the AUFRUHR text command (callback 0x601b50[16], name 0x601070[16]), the same message as 0x4aff30
   (AUFSTAND) with car 0: message 0x7d for the character ai_PlayerChar, today 8:00
   @deep only scheduling left (readable, 23 asm / 91 B): the original hoists ai_Time's first dword out of the TMX copy above the msg zeroing loop (mov edx,ds:0x770ae0 at 4b0fd6, before push edi / rep stos) and then stores the copy a,c,b,d (year 4b0ff7, +8 4b100b, +4 4b100f, word 4b1032) interleaved with the netId load, while ours keeps source order (netId load 4b0fe4, year 4b0fe9) and stores b,c,a,d (4b1006, 4b100c, 4b1018); car 0 and time.min 0 also come earlier here than in the original. diffclass: one 1-instruction code hunk (the hoist) + one code hunk of the same 8-instruction store permutation; the multiset test fails only because the offsets 0x2c/0x30/0x34 attach to different registers. Tried and reverted: TMX before msg.to (29 asm, worse). The same blocked pattern as the @deep 0x4afdd0 ai_CommandFest (hoisted ai_Time dword + a,c,b,d store order), so one vargen store-order pass should settle both. */

int ai_CommandRiot(void)
{
    AIMSG msg;

    memset(&msg, 0, sizeof(msg));
    msg.type = 0x7d;
    msg.to = ((AICHAR *)ai_PlayerChar)->netId;
    msg.time = ai_TimeT;
    msg.fromId = -1;
    msg.flags = 1;
    msg.car = 0;
    msg.time.hour = 8;
    msg.time.min = 0;
    cm_SendLongMessage(&msg);
    return 1;
}
