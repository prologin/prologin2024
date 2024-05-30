{ stdenv, pkgs, fetchurl }:

stdenv.mkDerivation rec {
  name = "html-export-templates";
  version = "3.5.2";

  src = fetchurl {
    url = "https://github.com/godotengine/godot/releases/download/3.5.2-stable/Godot_v3.5.2-stable_export_templates.tpz";
    sha256 = "sha256-AnFEDM/hQd28lB0THRPpXb/LBl3vrj4Qf/Z6g9YoXns=";
  };

  nativeBuildInputs = with pkgs; [ unzip ];

  phases = [ "installPhase" ];

  installPhase = ''
    mkdir -p "$out/share/godot/templates/${version}.stable"
    unzip $src
    cp templates/webassembly_* $out/share/godot/templates/${version}.stable
  '';
}
