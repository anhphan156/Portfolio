{
  description = "hello";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = {nixpkgs, ...}: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
  in {
    devShells."${system}".default = pkgs.mkShell {
      packages = with pkgs; [
        libxkbcommon
        raylib
        emscripten
      ];
      C_INCLUDE_PATH = "${pkgs.emscripten}/share/emscripten/cache/sysroot/include/";
      LD_LIBRARY_PATH = "${pkgs.emscripten}/share/emscripten/cache/sysroot/lib/";
    };
  };
}
