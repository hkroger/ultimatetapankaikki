Ultimate Tapan Kaikki
=====================

:point_right: **The original README.TXT can be found as [README.ORIG.TXT](./README.ORIG.TXT).**

This is the legendary 90s game Ultimate Tapan Kaikki, ported to
SDL2. It runs at least on macOS, Linux, FreeBSD and Windows.

The port is maintained by the [Suomipelit][suomipelit-gh] organization,
whose mission is to archive, port, and maintain classic Finnish freeware and
shareware games.  Feel free to [join the Suomipelit Slack][suomipelit-slack]
too!


Key bindings
------------

Player 1 and 2 game controls can be customized under `OPTIONS ->
PLAYER OPTIONS -> DEFINE KEYS 1/2`.

Game controllers are also supported but buttons need to be configured
in options before playing. No more than two controllers are recognized
at the same time. Analog inputs or navigating in menus is not
currently supported.

Other than player keys are hard-coded.

**Note:** Some Apple keyboards don't have a right ctrl key. If you
have such a keyboard, you have to change the player 1 shoot key before
playing.

**Moving in menus**

| Key | Description |
| --- | --- |
| Up/down arrow | Navigate |
| Return | Select |
| Esc | Go back |

**Player 1 game controls**

| Key | Description |
| --- | --- |
| Up arrow | Move forward |
| Down arrow | Move backward |
| Left arrow | Turn counter clockwise |
| Right arrow | Turn clockwise |
| Right ctrl | Shoot |
| Right shift | Change weapon |
| Right alt | Strafe |
| `,` | Starfe left |
| `.` | Starfe right |

When playing solo, you can use grave (left of `1`), `1` ... `9`, `0`,
and minus (right of `0`) to select the corresponding weapon. Grave
selects fist, `1` selects pistola, `2` selects shotgun, etc.

**Player 2 game controls**

| Key | Description |
| --- | --- |
| `w` | Move forward |
| `s` | Move backward |
| `a` | Turn counter clockwise |
| `d` | Turn clockwise |
| Tab | Shoot |
| Caps Lock | Change weapon |
| Grave | Strafe |
| `1` | Starfe left |
| `2` | Starfe right |

**Miscellaneous game controls**

| Key | Description |
| --- | --- |
| Esc, then `y` | Quit game |
| Space | Toggle map |
| F5 | Toggle frame rate |
| F12 | Capture screen shot |
| Left alt + Return | Toggle full screen |

**Shop controls**

| Key | Description |
| --- | --- |
| Arrow keys | Switch between items |
| Shoot key | Buy |
| Switch weapon key | Sell |
| Esc, then shoot key | Exit shop |
| Shift + Esc | Exit game immediately |


Differences compared to the original game
-----------------------------------------

This port is based on hkroger's published original sources at
https://github.com/hkroger/ultimatetapankaikki. Those sources are for
**v1.2 beta**, whereas the latest published version of the game is
**v1.21**, also called **TK321** (the released zipfile's name was
`TK321.zip`).

We've fixed all the bugs and differences between v1.2 beta and v1.21.

Changes required to port the game:

- When starting the game, it runs in a window. You can change to full
  screen and with left alt + return.

- Networking is now based on TCP/IP, originally it used IPX. When
  joining a network game, there's a new screen that allows typing the
  server address. Port is 8099.

- In-game screenshots are saved in BMP format as opposed to original
  PCX format.

New features:

- Pressing Esc in episode selection menu goes back to main menu.

- Pressing Shift + Esc anywhere in the game quits immediately.

- Game controller support.

Bugfixes:

- Many memory leaks and buffer overflows have been fixed.

- Many places in menus caused a high CPU load because of busy loops
  waiting for something. These have been fixed.

- Fixed player name changing to only accepts a-z and 0-9, space and
  `.`. Originally it accepted any keypress and produced a space for
  characters not available in the font.

- Clients now monitor server connection and quit to main menu if the
  server crashes.

- Player names and colors were mixed up in split screen mode.

Building from source
--------------------

**Requirements:**

- CMake
- C++ compiler: At least gcc, clang and Visual Studio are supported
- Libraries: SDL2, SDL2_mixer, SDL2_image, SDL2_net
  - On macOS, you can install these with Homebrew. `brew install sdl2 sdl2_mixer sdl2_image sdl2_net`
  - On Windows you can download these from SDL website

**Building:**

```shell
cmake .
cmake --build .
```

On Window, this produces `GAME.EXE` which you can run. On other
systems, the name of the executable is `tk3`.

On Windows, you may need to explicitly specify paths to your SDL libraries, like
```shell
cmake -DSDL2_PATH="C:\\<path>\\SDL2-2.0.9" -DSDL2_MIXER_PATH="C:\\<path>\\SDL2_mixer-2.0.4" -DSDL2_IMAGE_PATH="C:\\<path>\\SDL2_image-2.0.4" -DSDL2_NET_PATH="C:\\<path>\\SDL2_net-2.0.1" .
```
which produces project files for 32-bit target. For 64-bit target, use e.g. `cmake -G "Visual Studio 15 2017 Win64"`.


[suomipelit-gh]: https://github.com/suomipelit
[suomipelit-slack]: https://tinyurl.com/suomipelit-slack
