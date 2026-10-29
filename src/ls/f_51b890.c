/* 0x51b890, writes the two relation maps ai_Relations / ai_RelationMemory (0x300 x 0x300 bytes, row by row) */


int ls_WriteRelations(int f)
{
  int i;
  char *p;
  p = ((char *)ai_Relations);
  do {
    i = 0;
    do {
      LS_W(p + i, 1)
      i = i + 1;
    } while (i < 0x300);
    p = p + 0x300;
  } while ((int)p < (int)ai_Relations + 0x300 * 0x300);
  p = ((char *)ai_RelationMemory);
  do {
    i = 0;
    do {
      LS_W(p + i, 1)
      i = i + 1;
    } while (i < 0x300);
    p = p + 0x300;
  } while ((int)p < (int)ai_RelationMemory + 0x300 * 0x300);
  return 1;
}
