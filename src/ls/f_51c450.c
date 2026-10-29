/* 0x51c450, saves the history (hst.c) part of a save game: the texts-on byte hst_TextsOn, the offsets of the
   current text (hst_TextCurrent - hst_TextFirst) and of the next key-list entry (hst_KeyNext - hst_KeyFirst),
   the 0x10 bytes of the key hst_Key and the four 0x44-byte lists hst_Lists (used flag and eight id/value
   pairs each). Returns 1, or 0 when a write fails. */


int ls_WriteHistory(int f)
{
  int n;
  int i;
  int *list;
  int *entry;

  LS_W(&hst_TextsOn, 1)
  n = hst_TextCurrent - hst_TextFirst;
  LS_W(&n, 4)
  n = hst_KeyNext - hst_KeyFirst;
  LS_W(&n, 4)
  for (i = 0; i < 0x10; i++) {
    LS_W(hst_Key + i, 1)
  }
  /* list walks the lists by their e[0].v word (the word at +8 of each 0x44-byte record): list - 2 is the used
     flag, list - 1 and list are the id and value of each of the eight entries */
  for (list = &hst_Lists[2]; (int)list < (int)&hst_Lists[0x46]; list = list + 0x11) {
    LS_W(list - 2, 4)
    entry = list;
    for (i = 0; i < 8; i++) {
      LS_W(entry - 1, 4)
      LS_W(entry, 4)
      entry = entry + 2;
    }
  }
  return 1;
}
