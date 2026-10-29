/* unlinked */


void inflate_set_dictionary(s, d, n)
inflate_blocks_statef *s;
const Bytef *d;
uInt  n;
{
  zmemcpy(s->window, d, n);
  s->read = s->write = s->window + n;
}
