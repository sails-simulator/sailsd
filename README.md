Sailsd
======

Build status: [![BuildStatus](https://travis-ci.org/sails-simulator/sailsd.svg?branch=master)](https://travis-ci.org/sails-simulator/sailsd)

Compiling
---------

  1. Install the dependencies:

  __Fedora__: `sudo yum install cmake`

  __Debian/Ubuntu__: `sudo apt-get install cmake build-essential`

  2. Get source:

  ```bash
  $ git clone https://github.com/sails-simulator/sailsd.git
  ```

  3. Compile:

  ```bash
  $ cd sailsd
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make install
  ```

  4. Run with:

  ```bash
  $ ./build/sailsd
  ```

### OS X

  1. Install Xcode (not command line utilities) and accept the license.

  2. TODO

  3. TODO

  4. In this shell, start building stuff:

  ```bash
  $ git clone https://github.com/sails-simulator/sailsd.git
  $ cd sailsd
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make install
  ```

  5. Now run with:

  ```bash
  $ ./build/sailsd
  ```

### Windows

Install [MinGW](http://www.mingw.org/) and add the bin directory (defaults to
`C:\MinGW\bin`) to PATH.

Install [git](http://git-scm.com/download/win).

Get a copy of the source (using git bash):

```bash
$ git clone https://github.com/sails-simulator/sailsd.git
```

TODO


Usage
-----

TODO

Licence
-------

Copyright © 2014-2016 Louis Taylor

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
