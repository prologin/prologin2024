{ pkgs, stdenv, lib, html-export-templates}:

pkgs.stdenv.mkDerivation rec {
  src = ./godot;
  name = "prologin2024";
  version = "0.0.1";
  preset = "HTML5";

  nativeBuildInputs = with pkgs; [
    godot3-headless
  ];

  buildInputs = with pkgs; [
    xorg.libXext
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXrandr
    xorg.libXrender
    xorg.libX11
    xorg.libXi
    xorg.libXfixes
    libGLU
    zlib
    alsa-lib
    udev
    libglvnd
  ];

  postPatch = ''
    patchShebangs Scripts
  '';

  buildPhase = ''
    runHook preBuild
    export HOME=$TMPDIR
    mkdir -p $HOME/.local/share/godot
    ln -s ${html-export-templates}/share/godot/templates $HOME/.local/share/godot
    mkdir $out
    ${pkgs.godot3-headless}/bin/godot3-headless --export ${preset} $out/${name}
    runHook postBuild
  '';

  dontInstall = true;
}
