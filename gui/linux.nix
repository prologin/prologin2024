{ pkgs, stdenv, lib }:

stdenv.mkDerivation rec {
  src = ./godot;
  name = "prologin2024";
  version = "0.0.1";
  preset = "Linux/X11";

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
    patchShebangs scripts
  '';

  buildPhase = ''
    runHook preBuild
    export HOME=$TMPDIR
    mkdir -p $HOME/.local/share/godot
    ln -s ${pkgs.godot3-export-templates}/share/godot/templates $HOME/.local/share/godot
    mkdir $out
    ${pkgs.godot3-headless}/bin/godot3-headless --export "Linux/X11" $out/${name}
    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall
    mkdir -p $out/bin
    ln -s $out/${name} $out/bin
    interpreter=$(cat $NIX_CC/nix-support/dynamic-linker)
    patchelf \
      --set-interpreter $interpreter \
      --set-rpath ${lib.makeLibraryPath buildInputs} \
      $out/${name}
    runHook postInstall
  '';
}
