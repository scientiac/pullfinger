with import <nixpkgs> { };

stdenv.mkDerivation {
  name = "pullfinger";
  buildInputs = [
    gtk3
    pkg-config
    gcc
  ];
}
