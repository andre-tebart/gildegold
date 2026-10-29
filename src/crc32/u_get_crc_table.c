/* unlinked */
const uLongf * ZEXPORT get_crc_table()
{
#ifdef DYNAMIC_CRC_TABLE
  if (crc_table_empty) make_crc_table();
#endif
  return (const uLongf *)crc_table;
}
