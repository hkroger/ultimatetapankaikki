Ultimate Tapan Kaikki
=====================

:point_right: **The original README.TXT can be found as [README.ORIG.TXT](./README.ORIG.TXT).**

This is the legendary 90s game Ultimate Tapan Kaikki, ported to
SDL2. It runs at least on macOS, Linux, FreeBSD and Windows.

The port is maintained by the [Suomipelit][suomipelit-gh] organization,
whose mission is to archive, port, and maintain classic Finnish freeware and
shareware games.  Feel free to [join the Suomipelit Slack][suomipelit-slack]
too!

Build requirements
------------------

- CMake
- C++ compiler: At least gcc, clang and Visual Studio are supported
- Libraries: SDL2, SDL2_mixer, SDL2_net
  - On macOS, you can install these with Homebrew. `brew install sdl2 sdl2_mixer sdl2_net`
  - On Windows you can download these from SDL website


Building and running
--------------------

```shell
cmake .
make
./tk3
```

On Windows, you may need to explicitly specify paths to your SDL libraries, like
```shell
cmake -DSDL2_PATH="C:\\<path>\\SDL2-2.0.9" -DSDL2_MIXER_PATH="C:\\<path>\\SDL2_mixer-2.0.4" -DSDL2_NET_PATH="C:\\<path>\\SDL2_net-2.0.1" .
```
which produced project files for 32-bit target. For 64-bit target, use e.g. `cmake -G "Visual Studio 15 2017 Win64"`.



[suomipelit-gh]: https://github.com/suomipelit
[suomipelit-slack]: https://tinyurl.com/suomipelit-slack
