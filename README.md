Sails
=====

Sails is a simulator designed to test the AI of autonomous sailing robots. It
emulates the basic physics of sailing a small single sail boat

Currently a large work in progress.

Build status: [![BuildStatus](https://travis-ci.org/kragniz/sails.png?branch=master)](https://travis-ci.org/kragniz/sails)


Installing
----------

If you're running Nix, sails is available from the unstable channel:

```bash
$ nix-channel --add http://nixos.org/channels/nixpkgs-unstable
$ nix-channel --update
$ nix-env --install sails
```

API (draft)
-----------

Most of the interaction with sails takes place through the API. This is a
fairly standard REST+JSON based API, listening on port `3434`.

  - `/api`

  - `/api/boats`

    - `GET`: List all current boats.

    - `POST`: Launch a new boat.

  - `/api/boats/{name}`

    - `GET`: Show the current information about that boat.

        Example:
        ```json
        {
            "name": "kitty",
            "position": {
                "latitude": 4.231,
                "longitude": -1.232
            }
        }
        ```

    - `PATCH`: Update certain writable attributes of a boat. These are HTTP
      PATCH requests which conform to the JSON Patch format
      ([RFC6902](http://tools.ietf.org/html/rfc6902)).

    - `DELETE`: Sink the boat.

  - `/api/wind`

  - `/api/water`


Compiling
---------

### GNU/Linux

  1. Install the dependencies:

  __Fedora__: `sudo yum install cmake gtk3-devel librsvg2-devel libsoup-devel`

  __Debian/Ubuntu__: `sudo apt-get install cmake build-essential libgtk-3-dev librsvg2-dev libsoup-dev`

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
  $ make install
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
  $ make install
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
$ make install
```

Run with:

```bash
$ ./sails.exe
```


Usage
-----

Currently, the only way to interact with the boat is via the keyboard. This
will change once the API is implemented. Current controls are:

  - `e` and `r` - turn the rudder left and right
  - `s` - toggle whether the sail is held or relaxed
  - `space` - toggle camera mode from tracking and non-tracking

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
