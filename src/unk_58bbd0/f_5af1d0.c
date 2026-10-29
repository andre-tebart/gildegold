/* 0x5af1d0, the transport window (11968 B, ROUGH SKELETON): the modes 1 / 2 / 4 of the forwarders 0x5af1a0 ..
   0x5af1c0; a 0x1848-byte frame (__chkstk) with the 0x2d-dword table at +0x15c cleared, then one huge window
   build: the transport records of this unit (0x5ae870, 0x5ae6b0, 0x5ae8e0, 0x5ae9a0), the transport windows
   0x58d720 / 0x58dd00 / 0x58ee30 / 0x58f290, the controls 0x4731d0 / 0x475b80 / 0x475aa0 / 0x475f70, the
   amounts 0x5089c0 / 0x508a20, the texts 0x5176d0 / 0x52e330 / 0x521150, the sliders 0x475cb0, the carrier
   walk 0x53e400 / 0x53e300, the events 0x576040 / 0x576340 and the order 0x4c4880, on the window table
   0xf8aad0 and the key 0x62c418. The body is left as a stub: the Ghidra pseudocode (build/ghidra/5af1d0.c,
   1354 lines, its parameters lost) needs the disassembly, and VC6 spins on a first full draft of this size */

int ai_TransportWindow(int param_1, int param_2)
{
    int rec[0x2d];

    memset(rec, 0, sizeof(rec));
    return 0;
}
