/* 0x4f9710, the handler of the message 0x4c5e80 sends (0x4f9190): the tag at +0x24 'init' / 'set ' (stored
   byte-swapped, as the message carries it) copies the 9 ints to the record 0x91b480 (1); 'dsbl' / 'enbl' clear /
   set its first int (1); anything else 0 */

int ord_HandleMessageRecord(int *m)
{
    switch ((unsigned)m[9]) {
    case 0x656e626c:                    /* 'enbl' */
        ord_MessageRecordEnabled = 1;
        break;
    case 0x6473626c:                    /* 'dsbl' */
        ord_MessageRecordEnabled = 0;
        break;
    case 0x696e6974:                    /* 'init' */
    case 0x73657420:                    /* 'set ' */
        ORD_COPY_AT(offset ord_MessageRecord, m, 0x24);
        break;
    default:
        return 0;
    }
    return 1;
}
