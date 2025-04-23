{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    linuxPackages.kernel.dev
  ];
  LINUX_SRC = "${pkgs.linuxPackages.kernel.dev}";
}
