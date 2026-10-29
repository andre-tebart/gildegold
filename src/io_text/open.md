# io_text.c (new, layout 2026-10-23 session 3)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): text and name builders, the token engine
- Boundaries: separated (plausible) (layout 2026-10-23, 3rd session): new unit, tag "io_Text():" (0x6235d8 "io_Text(): Missing Random-Text (e.g. {r1})...", 0x623618 "io_Text(): too many icons before redraw!"). Evidence: .data: the block starts at "_STADTAUSWAHL_%s_INFO+0" (0x623564) right after f3init.c's city INI strings (0x623558) and runs through the name builders (0x62357c .. 0x6235bc: "%s$A%s" markup variants of 0x5143d0 / 0x514e90) and the text engine strings (0x6235c4 .. 0x623618) to "ls:tmp" 0x623654 (the ls unit); end: the language getter 0x519010 owns the global 0x62364c (the first global of ls.c). The text builders (character names with the grammar of 0x42c3b0: case 1 .. 6, genitive "'" / "s" after s z x sharp-s) and the token engine ("%s" "%G" "%N" "%T" "%i" "%f" "$C" ..) are the unit. Note the text engine 0x515960 / 0x5176d0 are SKELETONS (2912 / 2656 of 7536 / 6464 bytes: only the structure of the token loop and the simple tokens); the strings "%s %s" 0x60c7b8 / "%s" 0x5f8318 used here are the shared literals of earlier objs (the exe pools identical strings across objs)
- Range then: 0x5142e0..0x519010 (all drafted: 0x5142e0, 0x514360, 0x5143d0, 0x514e90, 0x515960, 0x5176d0)
- Was: tag "io_Text"

## Open work
- SKELETONS: 0x515960 (the token engine of the text window, 2912 of 7536 bytes) and 0x5176d0 (the same into a buffer, 2656 of 6464): only the structure of the token loop ("%" "$" "{" + digit + letter) and the simple tokens (s S G N T W X f i l t m a b e E u U B C D v w r) are drafted; the grammar of "%s" / "%S" (the 0x42c3b0 word forms by the case 1 .. 6, 0x5190a0 the language: some cases are skipped for the languages 0 / 4), the dot grouping of "%i" and the repeated string moves (the original inlines strlen / memmove / copy loops ~30 times) are summarised. Missing: the "%r" random text of a group, the case flags (param_2 .. param_10 of the original), the detail of the window record (0xf8f13c + 0x3c4 * window: +0x10 text buffer, +0x24c / +0x250 flags, +0x264 / +0x268 table of 0x41 arguments with count +0x368). 0x5176d0 is a copy of 0x515960 with the output buffer as first argument (and its own window-less state): generated from the skeleton, not read separately.
- ROUGH: 0x5143d0 (2144 of 2752 B) and 0x514e90 (2336 of 2752 B): the original repeats the strlen of the "ends in s / z / sharp s / x" test (an inline strlen per test), which the macro G30_ENDS_S only approximates.
- Names: all unk_Unk<addr>.

## layout 2026-10-23 (session 3)
- Found as a unit of its own (see notes/tus.md): the "io_Text():" error strings and the block of the "$A" markup name builders, between f3init.c (city INI) and ls.c (0x62364c).
- 0x5143d0 / 0x514e90 (id, case, kind, out): a character name text; id 0xfffe = the name at 0x143a618, < 0 or 0xffff = the empty text (0x1440ac8 / 0x143b3f4), the current player character gives the player's own name (7e3f40[6f1bc5], plus "s" in the genitive). The kinds: 1 name, 2 rank word + name, 3 rank + name + title, 4 trade + name, 5 the rank word, 6 title or name, 7 name + title, 8 / 9 genitive name with rank words. 0x514e90 handles the ids 0 .. 0x2ff only and writes "$A" (a text markup) between rank word and name; it appends the genitive twice for kind 4 names without words (a bug of the original).
- 0x5142e0 matches; 0x514360 (72 of 112 bytes differ).


## match-base 2026-10-23 (match worker)
- 0x5142e0 -> io_TownDisplayName, byte + readable: the display name of the town called name. The text table lookups take the txt.h names: txt_FindKey (0x42e770) and txt_Ptrs (0x143a1c0), declared locally with their address comments; the rt.h prototype and the callers in main.c / rt.c (they spelled it ai_Unk5142e0, an unresolved symbol before) renamed.
- 0x514360 -> io_TrailingDigits, byte + readable: the digit run ending at s[n] copied to out. The original's test is VC6's ctype.h `isdigit()` macro, `MB_CUR_MAX > 1 ? _isctype(c, _DIGIT) : _pctype[c] & _DIGIT`, so __mb_cur_max (0x62e3a0) is an int (the old `unsigned short` gave `cmp WORD ...,2` and the other branch shape) and _pctype (0x62e194) an `unsigned short *` (byte load at scale 2 for the & 4 test; the local `[s[i] * 2]` became `_pctype[s[i]]`). The tail is `memcpy(out, s + i, n - i + 1); out[n - i + 1] = 0;` with no len local: a `len` local moves the subtraction into n's register and costs one code hunk.
- 0x5143d0 -> io_CharacterName and 0x514e90 -> io_CharacterNameMarkup, every reference in io_text.c / f3gm.c / sv.c renamed; both escalated for behaviour and readability. No meaning error found: every case and the empty / player paths were read against the original (table bases 0x114 / 0x253 / 0x130 / 0x20d, the 0x3e00 flags, the "$A" formats, the double genitive of a kind 4 name without words). The code hunks are compiler shape: 2752 B original against our 2144 / 2336 (the G30_ENDS_S test expands 29 inline strlens against our 23, the buffers are initialised in the order A, C, B, the N / T pointers are recomputed per case, ebx / ebp are pushed only after the early return while ours pushes ebp at entry and shifts every stack slot by 4). Readability needs the typed 0x21c character record (raw offsets 0x9 / 0xd / 0x164..0x166 / 0x1cc) and the shared tables that ai.h / ahm.h / am.h declare under placeholder names (0x770b80 = ai_Chars, 0x7e3f40 the player slots, 0x6f1bc5 / 0x6f1bf8 the current player, 0x631090 = ch_EmptyName).
- 0x515960 and 0x5176d0 escalated (skeletons, 2912 / 7536 and 2656 / 6464 B): the %s / %S grammar (the txt_Field word forms by case 1 .. 6), the %r random text, the %i dot grouping, the case flags (param_2 .. param_10 of the original) and the window record (0xf8f13c + 0x3c4 * window) are unwritten; 0x5176d0 takes the output buffer as first argument and has no window record.

## match-base 2026-10-25 (base-4 worker, shard 4/6)

- 0x5176d0 unk_Unk5176d0 -> txt_Format; the ai_Unk5176d0 alias (877 references in ai.h / am.h / cm.h /
  rt.h and the units) folded into the same name.

## play 2026-10-28 (the %D date token)

- The `%D` case of io_PrintText called `gm_ClockToDate();` and `gm_Season();` with **no arguments** (implicit K&R
  declarations): the callee read the stack (`esi` = 1) and crashed while the tutorial dialog was drawn. The
  original passes the clock from the argument list and a 10-byte calendar record: `gm_ClockToDate((GCLOCK
  *)ap[0], record); ap++;`. Both copies (0x515960 and 0x5176d0) now do that and print the record's year word
  (`"%i"`). The original's text is `"%s %i"` with the season name from the 0x143a2f0 pointer table; that table
  is not declared or filled by any unit yet (no data.csv row), so the season is left out for now — the missing
  piece is the text-side filler, not this token.

## play 2026-10-28 (the scroll's missing text: the "$C" command and "%a")

- The `$C` command's field offsets were off: the draft did `w[0x92] = 0; memset(w + 0x9a - 1, 0, 0x100)`,
  but `w` is the D2WIN record +0x1c, so that is record +0x264 (`visible`) and record +0x280 (`active`) — it
  **deactivated the window it was clearing** and the text was never drawn. The original clears the text-command
  argument table: `mem(record +0x284, 0, 0x100)` (`w + 0x9a`, `args[0x40]`) and `record +0x384 = 0`
  (`w[0xda]`). The `d2_RemoveWindowObjects()` call was also argument-less (implicit K&R); it is
  `d2_RemoveWindowObjects(d2_LastWindow, 0)` (the original pushes d2_Wins[win].id and 0).
- The `%a` token was `sprintf(tmp, "%s", (char *)*ap++)`; its call sites pass an **int** (main_Unk5384e0's
  "$LRunden APs:$R+%a$A" passes the character field +0x190, default 4, which the save format also stores as an
  int). Reading it as a string crashed the day-change scroll in __output. It is `sprintf(tmp, "%i", *ap++)`.
- The day-change scroll (`misc\scroll_perga`, main_Unk5384e0's `io_PrintText(0x2051, clk, 0)`) now shows its
  German text. `%D` still prints only the year (the 0x143a2f0 season-name table is unset; see above).

## play 2026-10-29 (the io_PrintText return value and its redraw arguments)

- The return value was `*(short *)(w + 0x3)` = D2WIN.surf2's low word (w points at the window record +0x1c).
  The original's `movsx edi, WORD PTR [eax+0xf8f13c]` (eax = window * 0x3c4) reads **D2WIN.nobjs** (+0x1c): the
  callers pass it straight to d2_GetWinObj, so it is the index of the object that the text's `%ia[...]` is about
  to add (the redraw below has created it by the time d2_GetWinObj runs). The value must be captured **before**
  the redraw. With the old value every `d2_GetWinObj(view, 2, io_PrintText("%ia[%s]$N", ...))` returned object
  0; in the tutorial chapter window (0x57bce0) that marked object 0 instead of the five chapter entries, so
  their checkboxes stayed unchecked and "Tutorial starten" could not be pressed.
- The redraw calls were argument-less implicit K&R calls. The original passes
  `d2_DrawWindow(d2_LastWindow, d2_ScreenSurf)` (0x471a90) and `d2_RedrawView(*(int *)ctrl)` (0x474e80, the view
  id at D2VIEW+0), so the object the text adds was only created on the next pump frame and the returned index
  was still the empty count.

## play 2026-10-29 (the `$` commands and the `%s` rescan)

Two token-loop bugs kept the tutorial intro's button from working (its text is `"$Z%s"` with text id 0x7d =
`"%ia[Weiter]"`, i.e. the substitution itself carries a `%ia[...]` input token):

- **`%s` did not rescan its replacement.** The original continues the scan at the substitution
  (0x515c8a jumps to 0x515fce, which copies the string into the buffer at the token position and restarts the
  loop at 0x515a6e), so a replacement may contain further tokens. Ours did `txt_Subst` and then `pos++`, which
  skipped the inserted text: `%ia` was never seen, `flag9` stayed 0, the redraw that creates the object did not
  run and the returned `nobjs` was the count *before* the object existed. Now, in both copies, `pos = st - 1`
  after the `txt_Subst` of a plain `%s` (the tail `pos++` then continues at the substitution).
- **The switch default advanced twice over a `$` command.** The original's switch is on the letter after the
  marker, and for the `$` drawing commands (`A`, `Z`, `R`, `L`, `Y`, `[`, `]`) it jumps back to the scan with
  `pos` still on that letter (0x515ac5 -> 0x515a72); the letter is then consumed by the normal one-char scan.
  Ours did `pos++` in the default *and* in the tail, so `"$Z%s"`'s `%` was skipped and the window showed the
  literal `s` of `%s`. The default now only lets the tail advance, so a `$` command immediately before a token
  no longer eats it.

Both fixes are in io_PrintText (0x515960) and txt_Format (0x5176d0, the same engine into a buffer). They do not
change the remaining skeleton work (%r, the %i dot grouping, the case grammar). With them the tutorial dialog
draws its real `_BUTTON_RED` "Weiter" button, `d2_GetWinObj(view, 3, io_PrintText(...))` returns that object and
`ai_Unk577a00` advances the tutorial on a click.
