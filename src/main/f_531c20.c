/* 0x531c20, the engine shutdown (the low level parts) */

void main_ShutdownEngine(void)
{
    tb_Exit();
    vfs_Exit(1);
    tr_shutdown();
    mem_Exit();
    err_Exit();
    mov_Exit();
}
