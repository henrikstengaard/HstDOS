# HstDOS

Create DOS menu's fast with zero configuration of menu entries to start your favorite games and applications.

HstDOS can read directory and file names from a given directory and make it into a menu. Selecting directories will open a submenu and selecting files will start them. Files are filtered, so only executeable (.EXE, .COM) and batch (.BAT) files are shown.

![HstDOS](media/hstdos_v0.2_screenshot.png?raw=true)

# Installation

Install HstDOS with following steps:
1. Unzip to "C:\", so you will have it in "C:\HSTDOS".
2. Edit "C:\AUTOEXEC.BAT" and add directory with "HSTDOS.EXE" file to "SET PATH". For example, if your "SET PATH" is SET PATH=C:\DOS then it should be changed to SET PATH=C:\DOS;C:\HSTDOS.

# Usage

Use following keys in HstDOS:
- Arrow up: Move selection up.
- Arrow down: Move selection down.
- Arrow right: Open submenu, if entry is a directory.
- Arrow left: Go back to parent menu, if submenu is opened.
- Page up: Move selection one page up.
- Page down: Move selection one page down.
- Home: Move selection to first entry.
- End: Move selection to last entry.
- Enter: Select entry. If entry is a file or a directory with autostart, it will be started. If entry is a directory without autostart, it will be opened as a submenu.
- Esc: Quit HstDOS.

Selected entry is indicated with a blue line.

# Customizing menu entries

Directory menu entries can be customized by placing a HSTDOS.INI file in the directory. It supports following customizations:
- Title
- Autostart

Title can be set which can change a directory menu entry from ```JILL``` to ```Jill of the Jungle```.

Autostart can be set to automatically run a command, when the directory is selected. For the game Jill of the Jungle "JILL.EXE" starts the game and can be set as autostart.

Example of "HSTDOS.INI" placed in "JILL" directory:
```
[menu]
title=Jill of the Jungle
autostart=jill.exe
```

# Development

It's written in C and compiled with Turbo C v3.0 via DOSBox.

Bugs exist and it will crash as it's in a very early development state.