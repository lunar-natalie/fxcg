# Prizmatron: 2084

A multidirectional shooter for the Casio FX-CG50 calculator.

## Table of contents

* [Prizmatron: 2084](#prizmatron-2084)
    * [Table of contents](#table-of-contents)
    * [Documentation](#documentation)
    * [Usage](#usage)
    * [Building](#building)
        * [Requirements](#requirements)
        * [Configuration](#configuration)
        * [Standalone](#standalone)
        * [VS Code](#vs-code)
    * [Installation](#installation)
        * [Additional requirements](#additional-requirements)
        * [Setup](#setup)
        * [Install](#install)
            * [Standalone](#standalone-1)
            * [VS Code](#vs-code-1)
        * [Uninstall](#uninstall)
            * [Standalone](#standalone-2)
            * [VS Code](#vs-code-2)
    * [Code structure](#code-structure)
    * [Version control](#version-control)
    * [License](#license)

## Documentation

* POSIX shell commands to be run as user are denoted by shell code blocks
* Text to be substituted in shell commands is capitalised and encapsulated in
angle brackets
* 'Visual Studio Code' and 'Code - OSS' are abbreviated to VS Code

## Usage

After installing the add-in, the program should appear in the calculator's Main
Menu.

## Building

### Requirements

* [`libfxcg`](https://github.com/lunar-natalie/libfxcg)
* [`mkg3a`](https://github.com/tari/mkg3a)
* `make`
* `sh3eb-elf-binutils`
* `sh3eb-elf-gcc` with C17 support

### Configuration

* Add the following to `.env.mk`:

    ```Makefile
    FXCGSDK=<PATH_TO_SDK>
    ```

### Standalone

```Shell
make
```
* Output: `prizmatron.g3a`

### VS Code

* Run Build Task (`Build add-in`)
    * Default keybind: `shift + ctrl/cmd + b`
    * Output: `prizmatron.g3a`

## Installation

### Additional requirements

* `findmnt`
* `mount`
* `sudo`
* `umount`

### Setup

* Connect the calculator to your computer via USB
* Go to the 'Link' program if it hasn't automatically started
    * Select the Connection Mode 'USB Flash'
    * Select 'RECV'
* Note the path to the calculator's flash storage partition on your computer in
  `/dev/`
* Add the following to `.env.mk`:

    ```Makefile
    CGDEV=<PATH_TO_DEVICE>
    ```
* Follow the steps to install or uninstall the add-in. When prompted by the
  calculator, disconnect the calculator from your computer

### Install


#### Standalone

```Shell
sudo make install
```

#### VS Code

* Run Task (`Install add-in`)

### Uninstall

#### Standalone

```Shell
sudo make uninstall
```

#### VS Code

* Run Task (`Uninstall add-in`)

## Code structure

* Written in C
* Built with Makefile
* Source code is located in `src/`
* Indented with hard tabs of 8 characters in width
* Lines are no longer than 80 characters, unless readability is affected
* Uses [Doxygen](https://doxygen.nl/index.html) comments

## Version control

Commit messages should follow [this convention](https://www.conventionalcommits.org/)
for readability, and for any tools we may use in the future which depend on it.

## License

Copyright (c) 2022 Natalie Wiggins.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.

Written by Natalie Wiggins.

See `LICENSE` and `AUTHORS` for more information.
