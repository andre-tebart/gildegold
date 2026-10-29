
char d3_LodMissing = 0;  /* 0x6339e4, set when an object LOD is missing */
D3IOLOD d3io_Lod = { 2 };  /* 0x5fbf78, @original: d3_io_t.h declares D3IOLODBITS, the same 8 bits (mode : 7, shadow : 1) under its local name; both units only test .mode / .shadow and d3_io_t.c cannot include this header */
char d3io_StockPath[256] = {0};  /* 0x633a00 */
