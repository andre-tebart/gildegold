# Containment worklist (decompanion ab2c99c)

decompanion's containment checks found 158 errors in gildegold; 156 are fixed (git log from 9e752b55). Two D9
remain. `dcp build` + `dcp grade NAME` after each edit: no grade may fall.

- **M.contain** (ill-formed): our reference `&X + N` lies outside X's declared size (sizeof).
- **D7** (ill-formed): a view reads past its container's declared size.
- **D9** (contradiction): the original's code reads the padding our object gives a datum.

## Open

Both are pooled float literals of the original directly after one of our "constant" data definitions in .rdata:
our object pads the datum to the next symbol of its section, the original's linker put a literal there.

- `ai_FloatOne` (0x5eb780, cm.c, 1.0f): the original reads 255.0f at 0x5eb784 (53 uses: our code's literal
  255.0f, which compares by its bytes). In our cm object ai_FloatOne is in .data and ai_Unk5eb7b0 follows at
  +8 (a double, aligned); the original has 0x28 bytes between them (255.0f, ai_FloatZero, ...) and the whole
  region is .rdata. The named floats here (ai_FloatOne, ai_FloatZero, ...) stand for pooled literals the
  matching code needs as memory operands; how the original's .rdata interleaves its constants and literals
  decides the fix.
- `ai_CardBack` (0x5ed1bc, `static const volatile int` in include/unk_5d33b0.h): the original reads 77.0f at
  0x5ed1c0 (0x4a8f51). A header static has a copy in every unit that includes the header.

## Learned on the way

- Rewriting a memset over several globals as single stores costs the byte match (evt_Init): declare the block as
  one object and keep the names as its fields or views.
- unk_575860's ai_TutorialInitChapter2 / 3 flip their register tie-break on the unit's declaration set alone
  (the ledger's note at Chapter3). A struct of a new name around d2_DirtyRect cost Chapter2 its byte; keeping
  the container's symbol (`int d2_DirtyRectLeft[4]`) did not. The build clears Chapter3's @deep note whenever it
  comes out byte; put it back when it falls again (git show fc70ac1b^:src/unk_575860.c).
- dcp's M9 line cuts the original's string to our literal's length: dump the address for the whole string.
