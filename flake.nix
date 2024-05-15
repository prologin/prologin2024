{
  description = "Prologin 2024 Final Game";

  inputs = {
    stechec2.url =
      "git+https://gitlab.com/prologin/tech/tools/stechec2.git?ref=nix-stechec";
    futils.url = "github:numtide/flake-utils";
    # Do we need to stay on 22.11 because stechec2-nix is on 22.11?
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.11";
  };

  outputs = { self, stechec2, nixpkgs, futils }:
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
        };
      };

      multipleSystemsOutpus = eachDefaultSystem (system:
        let
          pkgs = import nixpkgs {
            inherit system;
            overlays = [ stechec2.overlay self.overlay ];
          };
        in
        rec {
          packages = { inherit (pkgs) prologin2024 ; };
          defaultPackage = self.packages.${system}.prologin2024;
          devShell = pkgs.mkShell {
            buildInputs =
              [ pkgs.stechec2 pkgs.waf pkgs.gtest ];
              inputsFrom = [ self.packages.${system}.prologin2024 ];
          };
        });
    in
    recursiveUpdate anySystemOutputs multipleSystemsOutpus;
}

