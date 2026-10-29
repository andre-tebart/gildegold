/* 0x425af0, turns the parent chain that ends at node `last` into a forward next chain: the chain
   nodes get parent = next from `last` back to the root.
   @deep behaviour only: the parameter mask, the parent-chain walk and the next-chain walk are the same instructions, the remaining hunks are register allocation (esi/edi/ecx) and the 0xffff constant register; readable met */

void path_Reverse(PMAP *map, unsigned int last)
{
    unsigned int cur, p;

    cur = last & 0xffff;
    if (map->nodes[cur].parent != 0xffff) {
        map->nodes[cur].next = 0xffff;
        while (cur != 0xffff) {
            p = map->nodes[cur].parent;
            if (p != 0xffff) {
                map->nodes[p].next = cur;
                p = map->nodes[cur].parent;
                last = p;
            }
            cur = p;
        }
        while (last != 0xffff) {
            map->nodes[last].parent = map->nodes[last].next;
            last = map->nodes[last].next;
        }
    }
}
