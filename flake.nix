{
  description = "Minesweeper";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/release-23.11";
  inputs.flake-utils.url = "github:numtide/flake-utils";


  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        libraries = [ pkgs.SDL2 pkgs.SDL2_ttf];
      in {

        devShells.default = pkgs.mkShell {
          nativeBuildInputs = [
            pkgs.gcc
            pkgs.pkg-config
            pkgs.cmake
            pkgs.ninja
            pkgs.meson
            pkgs.gdb
            pkgs.valgrind
          ];

          buildInputs = libraries;
        };

        packages.default = pkgs.stdenv.mkDerivation {
        pname = "minesweeper";
        version = "0.1";
        src = ./.;

        nativeBuildInputs = [
          pkgs.pkg-config
          pkgs.ninja
          pkgs.meson
        ];

        buildInputs = libraries;

        buildPhase = ''
          mkdir -p build
          cd build
          meson setup $src
          ninja
          '';

        installPhase = ''
          mkdir -p $out/bin
          cp sweeper $out/bin
          '';
        };

        apps.default = {
          type = "app";
          program = "${self.packages.${system}.default}/bin/sweeper";
        };
      }
    );
}
