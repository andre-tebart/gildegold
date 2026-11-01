"""dcp run step: the Windows compatibility layer the game needs (DWM8And16BitMitigation), for the staged exe.

HKCU\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers, value name = the exe's full path
(DCP_EXE). Written only when missing or different; --check only reports (exit 1 when it would write).
"""
import os
import sys
import winreg

KEY = r"Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"
LAYERS = "DWM8And16BitMitigation HIGHDPIAWARE"


def main() -> int:
    exe = os.environ["DCP_EXE"]
    check = "--check" in sys.argv
    access = winreg.KEY_READ if check else winreg.KEY_READ | winreg.KEY_WRITE
    try:
        k = winreg.OpenKey(winreg.HKEY_CURRENT_USER, KEY, 0, access) if check else winreg.CreateKeyEx(winreg.HKEY_CURRENT_USER, KEY, 0, access)
    except FileNotFoundError:
        print(f"compat_shim: {exe}: no Layers key, wants {LAYERS!r}")
        return 1
    with k:
        try:
            now = winreg.QueryValueEx(k, exe)[0]
        except FileNotFoundError:
            now = None
        if now == LAYERS:
            return 0
        if check:
            print(f"compat_shim: {exe}: {now!r}, wants {LAYERS!r}")
            return 1
        winreg.SetValueEx(k, exe, 0, winreg.REG_SZ, LAYERS)
        print(f"compat_shim: {exe} -> {LAYERS}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
