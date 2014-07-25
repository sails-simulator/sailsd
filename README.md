Sails
====

Simulator for autonomous sailing aquatic vehicles.

Build status: [![BuildStatus](https://travis-ci.org/kragniz/sails.png?branch=master)](https://travis-ci.org/kragniz/sails)

Compiling
---------

### GNU/Linux

Install the dependencies:

__Fedora__: `sudo yum install make gcc gtk3-devel librsvg2-devel`

__Debian/Ubuntu__: `sudo apt-get install make gcc libgtk-3-dev librsvg2-dev`

Get source:

```bash
$ git clone https://github.com/kragniz/sails.git
```

Compile:

```bash
$ cd sails
$ make
```

Run with:

```bash
$ ./sails
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
