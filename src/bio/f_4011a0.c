/* 0x4011a0, reads a zero-terminated string byte by byte (including the terminator) into s.
   stuck: the original tests the byte through s (cmp [eax],0 after mov eax,esi), VC6 tests it through p
   @deep B3: only code hunks 4011b5/4011ba (ours mov cl,[esi]; add esp; test cl,cl; mov eax,esi vs orig mov eax,esi; add esp; cmp [eax],0) + fill nop 4011cf; spell.py: no hunk matched the menu (escalate with diffclass verdict); this session tried *(s=p) as condition, bio_rd_c(f,++s), bio_rd_c(f,s=s+1), s=p before the call - same or shorter inc loop; note already tried while(*s)/*p/s[0]/!(*s==0)/uchar/volatile cast/for(;;)break/&&1/s++/++s/s=s+1/return s/extra pointer q=p. Left: VC6 CSE forwards the test load to p - register allocation/scheduling. */

void bio_rd_s(VFSFILE *f, char *s)
{
    char *p;

    s--;
    do {
        p = s + 1;
        bio_rd_c(f, p);
        s = p;
    } while (*s != 0);
}
