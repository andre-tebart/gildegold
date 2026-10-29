/* 0x4c0210, the tail of the queue (the sound queued last), 0 when the queue is empty */

G16SND *snd_QueueTail(void)
{
    G16SND *p;

    if (snd_Queue != 0) {
        p = snd_Queue;
        while (p->next != 0)
            p = p->next;
        return p;
    }
    return 0;
}
