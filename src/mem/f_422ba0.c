/* 0x422ba0 @deep byte work only (behaviour + readable met): 2 B / 4 asm lines, the imul operand order (ours mov esi,edx; imul esi,edi; the original mov esi,edi; imul esi,edx); grade.py diffclass verdict regs 6, spell.py finds no menu hunk; size*i, parentheses, casts, unsigned, an off temp, declaration orders and moving i = it->index all give the same code (notes/units/mem.md) */

/* returns the next used slot (the record after its in-use flag) of the pool chain walked by it, 0 at the end.
   The scan is `while (1) { if (*slot) break; ... }`: a plain while would be rotated by VC6. */
void *pool_Next(MPOOLITER *it)
{
    int i, size;
    int *slot;

    if (it && it->pool) {
        i = it->index;
        do {
            size = it->pool->size;
            if (it->left == 0)
                it->left = it->pool->used;
            slot = (int *)((char *)(it->pool + 1) + i * size);
            if (it->left > 0) {
                while (1) {
                    if (*slot)
                        break;
                    i++;
                    slot = (int *)((char *)slot + size);
                }
                if (--it->left <= 0) {
                    it->pool = it->pool->next;
                    it->index = 0;
                    it->left = 0;
                } else
                    it->index = i + 1;
                return slot + 1;
            }
            it->pool = it->pool->next;
        } while (it->pool);
    }
    return NULL;
}
