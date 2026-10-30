/* 0x4c0230, appends smp to the queue, played delay after the previous one @deep B draft, 105 code hunks, all one cluster: the new record's zeroing. Ours compiles memset(s,0,0x10) to four dword stores in an esi/edi frame; the original has VC6's generic inline memset (value and size materialised in [ebp-8]/[ebp-4], byte broadcast mov ah,al/shl/shr/add, alignment lea/and/sub, rep stosb/rep stosd/rep stosb) and the ebp frame that brings. Every other instruction corresponds (mem_Alloc(0x10,tag), sb_SetRestart(smp,1), s->smp/s->delay, snd_QueueTail, l->next/snd_Queue, return 1/0); same operation, spelling only. sizeof(G16SND) and 0x10 give the same four stores. Readable. */

int snd_AddToQueue(char *smp, unsigned int delay)
{
    G16SND *s;
    G16SND *l;

    if (smp != 0) {
        s = mem_Alloc(0x10, "f3snd: snd_AddToQueue");
        if (s != 0) {
            memset(s, 0, 0x10);
            sb_SetRestartC(smp, 1);
            s->smp = smp;
            s->delay = delay;
            l = snd_QueueTail();
            if (l != 0)
                l->next = s;
            else
                snd_Queue = s;
            return 1;
        }
    }
    return 0;
}
