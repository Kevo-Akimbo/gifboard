{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      qtEnv =
        with pkgs.qt6;
        env "qt-custom-${qtbase.version}" [
          qtdeclarative
          pkgs.kdePackages.layer-shell-qt
        ];
    in
    {
      packages.${system}.default = pkgs.callPackage ./. { };
      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = [
          pkgs.libxkbcommon
          pkgs.fontconfig
          pkgs.freetype
          pkgs.zlib
          pkgs.bzip2
          pkgs.libpng
          pkgs.brotli
          pkgs.expat
          pkgs.libglvnd

          pkgs.binutils
          pkgs.clang-tools
          pkgs.cmake
          qtEnv

          pkgs.rustPackages.cargo
          pkgs.rustPackages.clippy
          pkgs.rustPackages.rustc
          pkgs.rustPackages.rustfmt
        ];

        nativeBuildInputs = [
          pkgs.pkg-config
        ];

        LD_LIBRARY_PATH = "${pkgs.libxkbcommon}/lib";
        QMAKE = "${qtEnv}/bin/qmake";
      };
    };
}
