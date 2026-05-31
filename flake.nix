{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      qtEnv =
        with pkgs.qt6;
        env "qt-custom-${qtbase.version}" [
          qtdeclarative
          qtbase
          pkgs.kdePackages.layer-shell-qt
          pkgs.kdePackages.qtimageformats
        ];
      gitWrapper = pkgs.writeShellScriptBin "git" ''
        #!/usr/bin/env bash

        GIT="${pkgs.git}/bin/git"
        if [[ -t 0 || -p /dev/stdin ]]; then
          GIFBOARD_DIR="$($GIT rev-parse --show-toplevel)"
          function check_untracked_age() {
            local untracked_file="$1"
            local tracked_file="$2"
            if [[ -e "$untracked_file" ]]; then
              local untracked_age="$(stat -c %Y "$untracked_file")"
              local tracked_age="$(stat -c %Y "$tracked_file")"
              if (( $untracked_age > $tracked_age )); then
                echo -e "\x1b[31mWarning: Untracked $untracked_file is newer than tracked $tracked_file\x1b[0m" >&2
              fi
            fi
          }

          if [[ -e "$GIFBOARD_DIR/untracked" ]]; then
            check_untracked_age "$GIFBOARD_DIR/untracked/main.qml" "$GIFBOARD_DIR/gifboard-qml/qml/main.qml"
          fi
        fi


        exec "$GIT" "$@"
      '';
    in
    {
      packages.${system} = {
        gifboard = pkgs.callPackage ./. { };
        default = self.packages.${system}.gifboard;
      };
      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = [
          pkgs.libxkbcommon
          pkgs.libglvnd
          pkgs.libxcb
          pkgs.libx11
          qtEnv
        ];

        nativeBuildInputs = [
          qtEnv
          pkgs.libx11.dev

          pkgs.pkg-config
          pkgs.binutils
          pkgs.clang-tools
          pkgs.cmake
          pkgs.clang
          pkgs.rustPackages.cargo
          pkgs.rustPackages.clippy
          pkgs.rustPackages.rustc
          pkgs.rustPackages.rustfmt
          pkgs.rust-analyzer
          pkgs.lld
          pkgs.ninja
          pkgs.sccache
          pkgs.xdpyinfo
          pkgs.dwm
          gitWrapper
        ];

        LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
          pkgs.libxkbcommon
          pkgs.libxcb
          pkgs.libx11
          qtEnv
          pkgs.stdenv.cc.cc.lib
        ];
        QMAKE = "${qtEnv}/bin/qmake";
        hardeningDisable = [ "fortify" ];
        RUSTC_WRAPPER = "${pkgs.sccache}/bin/sccache";
      };
    };
}
