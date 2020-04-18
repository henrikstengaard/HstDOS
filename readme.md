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

# Navigation

## Keyboard

## Mouse

HstDOS detects if a mouse present and supports following navigation:

| Navigation | Description |
| --- | --- |
| Start or enter | Left click on menu entry to start or enter the menu entry. If menu entry is a directory, it will be opened as a submenu. If menu entry is back entry as first menu entry in a submenu, it will go back to parent menu. If autostart is configured for the menu entry, it will be started. |
| Enter | Right click on menu entry to enter the menu entry. If menu entry is a directory, it will be opened as a submenu. If menu entry is back entry as first menu entry in a submenu, it will go back to parent menu. |
| Page up | Left click on top shadow above menu entries. |
| Page down | Left click on bottom shadow below menu entries. |

# Configuration

HstDOS can be configured by adding a ```HstDOS.ini``` file in same directory as ```HstDOS.exe``` and allows changing texts and colors used by HstDOS to show menus.

The default ```HstDOS.ini``` file has following settings.

```ini
[hstdos]
backText=< Back >
onlyExecutableFiles=1
titleBackgroundColor=1
titleTextColor=15
menuBackgroundColor=0
menuTextColor=7
selectedBackgroundColor=5
selectedTextColor=15
shadowBackgroundColor=0
shadowTextColor=8
```

Description of settings:
- backText: Set text shown as first menu entry in submenus to nagivate back.
- onlyExecutableFiles: If set to 1, HstDOS will only show executeable files: .EXE, .COM, .BAT.
- titleBackgroundColor: Set background color of the title.
- titleTextColor: Set text color of the title.
- menuBackgroundColor: Set background color of the menu entries.
- menuTextColor: Set text color of the menu entries.
- selectedBackgroundColor: Set background color of the selected menu entry.
- selectedTextColor: Set text color of the selected menu entry.
- shadowBackgroundColor: Set background color of the shadow shown below the title and in the bottom of the screen.
- shadowTextColor: Set text color of the shadow shown below the title and in the bottom of the screen.

HstDOS can use any of the following 16 colors for either the text color or the background color.

| Color | Value |
| --- | --- |
| BLACK | 0 |
| BLUE | 1 |
| GREEN | 2 |
| CYAN | 3 |
| RED | 4 |
| MAGENTA | 5 |
| BROWN | 6 |
| LIGHTGRAY | 7 |
| DARKGRAY | 8 |
| LIGHTBLUE | 9 |
| LIGHTGREEN | 10 |
| LIGHTCYAN | 11 |
| LIGHTRED | 12 |
| LIGHTMAGENTA | 13 |
| YELLOW | 14 |
| WHITE | 15 |

In a DOS console the colors will be shown like the following screenshot.

![HstDOS](media/colors.png?raw=true)

# Menu entries

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