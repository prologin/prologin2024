{
  description = "Prologin 2024 Final Game";

  inputs = {
    stechec2.url =
      "git+https://gitlab.com/prologin/tech/tools/stechec2.git?ref=nix-stechec";
    futils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
    nixgl.url = "github:guibou/nixGL";
  };

  outputs = { self, stechec2, nixpkgs, futils, nixgl }:
    let
      inherit (nixpkgs) lib;
      inherit (lib) recursiveUpdate;
      inherit (futils.lib) eachDefaultSystem;

      anySystemOutputs = {
        overlay = final: prev: rec {

          prologin2024 = final.mkStechec2Game {
            name = "prologin2024";
            game = ./.;
            version = "1.0";
          };

          prologin2024-gui-linux = final.callPackage ./gui/linux.nix { };

          html-export-templates = final.callPackage ./gui/html-templates.nix {};

          prologin2024-gui-html = final.callPackage ./gui/html.nix { html-export-templates =  final.html-export-templates; };

        };

      };

      multipleSystemsOutpus = eachDefaultSystem (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ stechec2.overlay self.overlay nixgl.overlay ];
          };

        in
        rec {
          packages = { inherit (pkgs) prologin2024 prologin2024-gui-linux prologin2024-gui-html; };

          defaultPackage = self.packages.${system}.prologin2024;

          devShell = pkgs.mkShell {
            buildInputs =
              [ pkgs.stechec2 pkgs.godot pkgs.nixgl.nixGLIntel ];
          };
        });


    in
    recursiveUpdate anySystemOutputs multipleSystemsOutpus;
}

