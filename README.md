# cons

## Installation

- [Mac Installation](#mac_installation)
- [Windows Installation](#windows_installation)
- [Linux Installation](#linux_installation)

## Mac Installation

## Windows Installation

## Linux Installation

## Building From Source

- [Mac Build](#mac_and_linux)
- [Windows Build](#windows_build)
- [Linux Build](#mac_and_linux)

# Mac and Linux

1. Clone `git clone https://github.com/coffeebe4code/cons.git`
2. Ensure you have `clang` or `gcc` installed, and is a relatively newer version. `gcc --version` or `clang --version`
  - many versions should work, the project is compiled with `-Werror -Wall -Wextra`. And various versions of gcc and clang will have different criteria which may result in errors for the source code.
  - all have been very simple to change, see [Contributing](#contributing) for a quick way to fix the issue and commit to source. If you do not have the time, submitting a bug report for the OS, gcc version, and the list of errors reported, I will fix them asap.

3. You must have pthreads installed and visible for your compiler.

4. Bootstrap `nobuild`, the build system for `cons`.
  - At the root of the project run `gcc -Wall -Werror -Wextra -pthread -O3 -o ./nobuild ./nobuild-gcc.c`
  - be sure to substitube `clang` as the command and `nobuild-clang.c` if you intend to build with clang

5. build the project with `./nobuild` this will build the debug version. You can skip tests or run the release build with `./nobuild -s -r`

All binaries and libraries will be placed in the `./target` directory.

- NOTE:: Mac debug version has an issue with debugging in gdb.

# Windows Build

We must set up the Mingw64 toolchain for building source as windows binaries.

1. Download the `msys2` system. I recommend using their installer on their [website](https://www.msys2.org).
  - Their installation guide found on the main page is nearly everything we will need.
  - After step 7 in previously linked mys2 guide, run `pacman -S git` so that we can clone the source code for `cons`.
2. Clone the `cons` repository. `git clone https://github.com/coffeebe4code/cons.git`
3. This will be added to the `home` folder within the `msys2` environment. If you changed directories or created one, just take note of where this was done.
4. You can proceed to step 8 on previously linked msys2 installation guide. I typically just go to where msys2 was installed, and find the mingw64 terminal and run it from there. I will also pin this new terminal to the task bar.
5. With the Mingw64 shell open, change your directory to where you installed cons, `cd ~/cons` for those who ran the git clone at the home or default directory.
6. Now you can bootstrap the build system run `gcc -Wall -Werror -Wextra -pthread -O3 -o ./nobuild ./nobuild-win.c`
7. You may now run any `./nobuild` command, add `-s` to skip tests, and/or `-r` to build the release version. the release version is significantly faster than the debug version.
8. The windows version of cons is completely self contained and does not need any dll's to run on a windows system, it does not need any fancy directory, just ensure to add any of the binaries you wish to the PATH System Environment variable.
  - if you used the default everything you would need to add `C:\\msys2\home\cons\target` to your PATH variable in your System Environment Variables.
