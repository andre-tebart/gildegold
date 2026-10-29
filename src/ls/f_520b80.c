/* 0x520b80, takes the save game that arrived over the network (the buffer 0xf31140, length 0xa68d28, wait for 0xf81220 to reach it), opens it as a memory file (0x4311d0), reads the header and the globals like 0x51c950 (only when the header flag has bit 2: the network start flag, and the version is below 0x1005b), the readers 0x51d3d0, 0x51d550, 0x51db00, 0x51de70 (the characters of the kind 6 become kind 5), 0x51f080, 0x4ea7a0 (from 0x10045), 0x51d2d0 (unless the header flag has bit 4) and 0x5206b0. The first failing step closes the file, frees the buffer and returns 0. Returns 1. ROUGH @deep B3 code hunks; fixed the 2-byte clock field (0x75e13c stored as word at 0x520cf9/0x520d14, our int view overran the 14-byte copy), the v local removed (the original reads ls_Version directly at 0x520d2f/0x520d50), unsigned ai_Unkf81220/ai_Unka68d28 (jae at 0x520ba1) and the unsigned short 0xffff character test (0x520de9); 443/752 bytes, 72 asm lines left: the hoisted push esi of the failure arms (passing fh to vfs_fclose is correct - push esi at 0x520db3/0x520dd3 - but made unitdiff WORSE, reverted for match-deep), the 0x100 vs 0xfc frame and the 0x1003d arm layout; far draft, no menu pass */


int ls_ReceiveGame(void)
{
    int fh, i;
    int hdr[0x40];
    char *p;

    while (ai_Unkf31140 == 0 || ai_Unkf81220 < ai_Unka68d28) {
        sv_PumpMessages();
        sv_PumpNet();
    }
    fh = vfs_MemOpen(ai_Unkf31140, ai_Unka68d28, "rb");
    if (fh == 0) {
        mem_Free(ai_Unkf31140);
        ai_Unkf31140 = 0;
        return 0;
    }
    ai_PlayerChar = 0;
    ls_ReadSaveHeader(fh, hdr);
    if ((((char *)hdr)[4] & 2) != 0 && hdr[0] <= 0x1005a) {
#define LS_RF(p, sz) ls_Read((int)(p), (sz), 1, fh)
        if (!LS_RF(&ai_NextObjectId, 4) || !LS_RF(&ai_Unk6f2618, 4) || !LS_RF(ai_Time, 0xe)
            || !LS_RF(&ai_OwnPlayer, 1) || !LS_RF(&ai_PlayerChar, 4) || !LS_RF(&ai_Unk6f1bb8, 4)
            || !LS_RF(&ai_Unk6f1bbc, 4) || !LS_RF(&ai_CharCount, 4)) {
            vfs_fclose(fh);
            goto fail;
        }
        ai_Unk75e130 = *(int *)ai_Time;
        ai_Unk75e134 = *(int *)(ai_Time + 4);
        ai_Unk75e138 = *(int *)(ai_Time + 8);
        ai_Unk75e13c = *(short *)(ai_Time + 0xc);
        LS_RF(&ai_Unk6f1bcc, 4);
        LS_RF(ai_UpdateStart, 0x18);
        if (ls_Version >= 0x10030)
            LS_RF(&ai_Unk61a658, 4);
        if (ls_Version < 0x1003d)
            ai_Unk6f1b88 = 1000000;
        else
            LS_RF(&ai_Unk6f1b88, 4);
        if (ls_ReadObjects(fh) == 0) {
            ((int (*)())vfs_fclose)();
            goto fail;
        }
        if (ls_ReadBuildings(fh) != 0) {
            if (ls_ReadPlayerRecords(fh) == 0) {
                ((int (*)())vfs_fclose)();
                goto fail;
            }
            if (ls_ReadCharacters(fh) == 0) {
                vfs_fclose(fh);
                goto fail;
            }
            p = ai_Chars[0] + 2;
            do {
                if (*(unsigned short *)(p - 2) != 0xffff && *p == 6)
                    *p = 5;
                p += 0x21c;
            } while ((int)p < (unsigned)((unsigned)ai_Chars + 0x65402));
            if (unk_Unk51f080(fh) != 0) {
                if (ls_Version >= 0x10045 && amt_LoadAemter(fh, ls_Version) == 0) {
                    vfs_fclose(fh);
                    goto fail;
                }
                if ((((char *)hdr)[4] & 4) == 0 && unk_Unk51d2d0(fh) == 0) {
                    vfs_fclose(fh);
                    goto fail;
                }
                if (ls_FixupAfterLoad() != 0) {
                    vfs_fclose(fh);
                    mem_Free(ai_Unkf31140);
                    ai_Unkf31140 = 0;
                    return 1;
                }
            }
        }
    }
    vfs_fclose(fh);
fail:
    mem_Free(ai_Unkf31140);
    ai_Unkf31140 = 0;
    return 0;
}
