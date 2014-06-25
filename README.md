sail
====

Simple simulator for small sailing boats.

Compiling
---------

### GNU/Linux

Install the dependencies:

Fedora: `sudo yum install make gcc gtk3-devel librsvg2-devel`

Debian/Ubuntu: `sudo apt-get install make gcc libgtk-3-dev`

Get source:

```bash
$ git clone https://github.com/kragniz/sail.git
```

Compile:

```bash
$ cd sail
$ make
```

Run with:

```bash
$ ./sail
```

### Windows

Install [MinGW](http://www.mingw.org/) and add the bin directory (defaults to
`C:\MinGW\bin`) to PATH.

Install GTK+ from the [all-in-one bundle](http://www.gtk.org/download/win32_tutorial.php).

Install [git](http://git-scm.com/download/win).

Get a copy of the source (using git bash):

```bash
$ git clone https://github.com/kragniz/sail.git
```

Compile using make:

```bash
$ cd sail
$ make
```

Run with:

```bash
$ ./sail.exe
```
