"""dcp run step: game.ini of the run directory in a window, intro and sound off (the copy only, never the install's).
GG_INTRO=1 keeps the intro."""
import os
import re
import sys
from pathlib import Path

ZERO = ("msx", "sfx", "weather", "master_vol", "sfx_vol", "msx_vol", "speech_vol")


def main() -> int:
    ini = Path(os.environ["DCP_RUN_DIR"]) / "game.ini"
    intro = os.environ.get("GG_INTRO") == "1"
    lines = []
    for line in ini.read_text(encoding="latin-1").splitlines():
        key = line.split("=", 1)[0].strip().lower()
        if key == "bildmodus":
            line = "Bildmodus=DIRECTWINDOW"
        elif key == "show_intro":
            line = f"show_intro={int(intro)}"
        elif key in ZERO:
            line = key + "=0"
        lines.append(line)
    ini.write_text("\r\n".join(lines) + "\r\n", encoding="latin-1", newline="")
    bad = [ln for ln in lines if re.match(r"(?i)^(bildmodus=(?!DIRECTWINDOW$)|(msx|sfx|weather)=[^0]|(master|sfx|msx|speech)_vol=[^0])", ln)]
    if bad:
        print("game_ini: still fullscreen or sound on: " + ", ".join(bad), file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
