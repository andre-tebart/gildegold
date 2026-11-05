"""dcp run step: the run directory's own game.ini (the copy only, never the install's), kept between runs.

dcp stages game.ini once and keeps it; this step sets the run defaults (intro off, the volumes of the options menu
at 0) only the first time, when the copy has no MARK line yet, so what the options menu saves stays saved. Every
run it enforces the window mode (Bildmodus=DIRECTWINDOW) and the sound switches msx / sfx / weather = 1: the game
reads them only at startup and the options menu cannot change them, so sound is the volumes' business alone.
GG_INTRO=0/1 sets the intro and keeps it. Delete build/run/game/game.ini to stage a fresh copy of the install's."""
import os
import sys
from pathlib import Path

MARK = "; dcp run: own copy, the options persist (tools/run/game_ini.py)"
SWITCHES = ("msx", "sfx", "weather")
VOLUMES = ("master_vol", "sfx_vol", "msx_vol", "speech_vol")


def main() -> int:
    ini = Path(os.environ["DCP_RUN_DIR"]) / "game.ini"
    lines = ini.read_text(encoding="latin-1").splitlines()
    first = MARK not in lines
    v = os.environ.get("GG_INTRO")
    intro = (v == "1") if v not in (None, "") else (False if first else None)
    out = [MARK] if first else []
    for line in lines:
        key = line.split("=", 1)[0].strip().lower()
        if key == "bildmodus":
            line = "Bildmodus=DIRECTWINDOW"
        elif key == "show_intro" and intro is not None:
            line = f"show_intro={int(intro)}"
        elif key in SWITCHES:
            line = key + "=1"
        elif key in VOLUMES and first:
            line = key + "=0"
        out.append(line)
    if out != lines:
        ini.write_text("\r\n".join(out) + "\r\n", encoding="latin-1", newline="")
    if not any(ln.strip().lower() == "bildmodus=directwindow" for ln in out):
        print("game_ini: no Bildmodus=DIRECTWINDOW line, the game would start fullscreen", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
