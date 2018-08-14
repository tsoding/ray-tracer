with import <nixpkgs> {}; {
    raytracerEnv = stdenv.mkDerivation {
        name = "raytracer-env";
        buildInputs = [ stdenv
                        gcc7
                        imagemagick
                        sfml
                        cmake
                      ];
    };
}
