Sails
====

Sails is a simulator designed to test the AI of autonomous sailing robots. It
emulates the basic physics of sailing a small single sail boat

Currently a large work in progress.

Build status: [![BuildStatus](https://travis-ci.org/kragniz/sails.png?branch=master)](https://travis-ci.org/kragniz/sails)

Compiling
---------

### GNU/Linux

  1. Install the dependencies:

  __Fedora__: `sudo yum install cmake gtk3-devel librsvg2-devel`

  __Debian/Ubuntu__: `sudo apt-get install cmake build-essential libgtk-3-dev librsvg2-dev`

  2. Get source:

  ```bash
  $ git clone https://github.com/kragniz/sails.git
  ```

  3. Compile:

  ```bash
  $ cd sails
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ cd ..
  ```

  4. Run with:

  ```bash
  $ ./build/sails
  ```

### OS X

  1. Install Xcode (not command line utilities) and accept the license.
  2. Install JHBuild (a tool written for building large GNOME applications):

  ```bash
  $ wget https://git.gnome.org/browse/gtk-osx/plain/gtk-osx-build-setup.sh
  $ sh gtk-osx-build-setup.sh
  $ alias jhbuild="~/.local/bin/jhbuild"
  $ jhbuild bootstrap
  $ jhbuild build meta-gtk-osx-bootstrap
  $ jhbuild build meta-gtk-osx-gtk3
  ```

  3. Now, JHBuild should be all ready to compile GTK+3 applications. To get a
     sane build environment, enter the jhbuild shell:

  ```bash
  $ jhbuild shell
  ```

  4. In this shell, start building stuff:

  ```bash
  $ git clone https://github.com/kragniz/sails.git
  $ cd sails
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  $ cd ..
  ```

  5. Now run with:

  ```bash
  $ ./build/sails
  ```

### Windows

Install [MinGW](http://www.mingw.org/) and add the bin directory (defaults to
`C:\MinGW\bin`) to PATH.

Install GTK+ from the [all-in-one bundle](http://www.gtk.org/download/win32_tutorial.php).

Install [git](http://git-scm.com/download/win).

Get a copy of the source (using git bash):

```bash
$ git clone https://github.com/kragniz/sails.git
```

Compile using make:

```bash
$ cd sails
$ make
```

Run with:

```bash
$ ./sails.exe
```

Licence
-------

Copyright © 2014 Louis Taylor

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
