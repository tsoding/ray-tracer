[![Build Status](https://travis-ci.org/tsoding/ray-tracer.svg?branch=master)](https://travis-ci.org/tsoding/ray-tracer)
# Ray Tracer in pure (99.999%) C++

![](https://i.imgur.com/WyFWDAP.png)
![](https://i.imgur.com/a/MkuvNiA.png)

## Quick Start

```console
$ nix-shell                            # Only if you are on NixOS
$ mkdir build && cd build/
$ cmake ..
$ make
$ ./ray-tracer ../scene.txt            # for preview mode
$ ./ray-tracer ../scene.txt output.ppm # for final rendering
$ feh output.ppm
```
