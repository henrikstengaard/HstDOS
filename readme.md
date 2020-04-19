# HstDOS

Create DOS menu's fast with zero configuration of menu entries to start your favorite games and applications.

HstDOS can read directory and file names from a directory and turn it into a menu. Selecting directories will open them as a submenu and selecting executable files will start them.

![HstDOS](media/hstdos_v0.4_screenshot.png?raw=true)

# Features

HstDOS comes with following features:
- Turn any directory into a menu.
- Keyboard and mouse to navigate menus.
- Optional customization of menu entries by adding "HSTDOS.INI" files to set title and autostart.
- Change colors in HstDOS menus to give it a personal look and feel.
- Requires ~90KB of conventional memory to run.
- Uses batch file start HstDOS and run selected game or application. This is to have maximum free memory to run game or application as HstDOS quits after menu entry is selected.

# Installation

Install HstDOS with following steps:
1. Unzip to "C:\\", so you will have files extracted to "C:\HSTDOS" and "C:\GAMES".
2. Edit "C:\AUTOEXEC.BAT" and add directory with "HSTDOS.EXE" file to "SET PATH". For example, if your "SET PATH" is ```SET PATH=C:\DOS``` then it should be changed to ```SET PATH=C:\DOS;C:\HSTDOS```.

Adding the directory with "HSTDOS.EXE" and "HD.BAT" to "SET PATH" in "AUTOEXEC.BAT" enables HstDOS to be started from any directory.

HstDOS can be installed to any other directory than "C:\HSTDOS" with following changes:
1. Edit "AUTOEXEC.BAT" and "SET PATH" to include the directory with "HSTDOS.EXE" and "HD.BAT".
2. Restart to apply changes to "AUTOEXEC.BAT".

The directory HstDOS reads menu entries from can be changed to any other directory than "C:\GAMES" with following changes:
1. Edit "HD.BAT" and goto line with ```hstdos.exe /d c:\games```.
2. Replace ```c:\games``` with directory to show as a menu in HstDOS.

# Usage

Start HstDOS by typing ```hd``` and press enter.

Navigate to the game or application to start.

HstDOS will quit, when starting a game or application will to free memory.

After quitting game or application, HstDOS will start again ready to start next game or application.

# Navigation

Keyboard and mouse can be used to navigate menus in HstDOS.

The selected menu entry is indicated with a magenta line in the middle of the screen.

Menus in HstDOS supports following navigation:

| Navigation | Description |
| --- | --- |
| Start | Start the selected menu entry, if it's an executable file or a directory with autostart. If menu entry is a directory, it will be opened as a submenu. If menu entry is a back entry (```< Back >```) in a submenu, it will go back to parent menu. |
| Browse | Browse the selected menu entry by entering it as a submenu, if it's a directory. |
| Back | Returns back to parent menu from submenu. |
| One up | Move selected menu entry one up. |
| One down | Move selected menu entry one down. |
| Page up | Moved selected menu entry a page up. |
| Page down | Moved selected menu entry a page down. |
| First | Move selected to first menu entry. |

## Keyboard

Menus in HstDOS supports following keyboard presses:

| Keyboard press | Navigation |
| --- | --- |
| Arrow up | One up |  
| Arrow down | One down |  
| Arrow right | Browse |  
| Arrow left | Back |
| Enter | Start |
| Page up | Page up |
| Page down | Page down |
| Home | First |
| F1 | Show about |
| Esc | Quit HstDOS |

## Mouse

Menus in HstDOS supports following mouse actions:

| Mouse action | Navigation |
| --- | --- |
| Move mouse | Moves mouse cursor on screen |
| Left click on menu entry | Start |
| Right click on menu entry | Browse |
| Left click on top shadow above menu entries | Page up |
| Left click on bottom shadow below menu entries | Page down |

# Menu entries

Menu entries can be customized placing a "HSTDOS.INI" file directories and it can be used to change following:
- Title
- Autostart

Title can be set which can change a directory menu entry from for example ```JILL``` to ```Jill of the Jungle```.

Autostart can be set to automatically run a command, when the menu entry is started. For the game Jill of the Jungle "JILL.EXE" starts the game and can be set as autostart.

Example of "HSTDOS.INI" placed in "C:\GAMES\JILL" directory to customize menu entry:
```ini
[menu]
title=Jill of the Jungle
autostart=jill.exe
```

Example of "HSTDOS.INI" placed in "C:\GAMES" directory to customize menu title:
```ini
[menu]
title=My favorite games
```

# Configuration

HstDOS can be configured by adding a "HSTDOS.INI" file in same directory as "HSTDOS.EXE" and allows changing texts and colors used by HstDOS to show menus.

The default "HSTDOS.INI" file has following settings.

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
- backText: Set text shown as first menu entry in submenus to navigate back.
- onlyExecutableFiles: If set to 1, HstDOS will only show executeable files: .EXE, .COM, .BAT. If set to 0, all files are shown.
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

# Development

It's written in C and compiled with Turbo C v3.0 via DOSBox.

HstDOS is build from a Powershell console by running "build.ps1" script. It requires DOSBox is installed.