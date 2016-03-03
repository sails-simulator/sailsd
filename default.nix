with import <nixpkgs> {}; {
  sailsdEnv = stdenv.mkDerivation {
    name = "sailsd";
    buildInputs = [ stdenv cmake pkgconfig valgrind astyle
    jansson man-pages gdb cmocka ];
  };
}
