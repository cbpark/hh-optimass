# hh-optimass

## Build prerequisites

* C++ compiler supporting C++11 features ([Clang](http://clang.llvm.org/cxx_status.html) >= 3.3, [GCC](https://gcc.gnu.org/projects/cxx-status.html) >= 4.8.1),
* [CLHEF](https://github.com/cbpark/CLHEF),
* [ROOT](https://root.cern.ch),
* OptiMass/alm_base.

Adjust `CLHEF` and `OPTIMASS` to their installation paths in [`Makefile`](Makefile). For instance,

```
CLHEF=/clhef/path OPTIMASS=/optimass/path make
```

and check if `root-config` is in your `PATH`.

## Usage

Assuming the input file is `input.lhe` and the file to save the output is `output.dat`,

```
./bin/hh_optimass_parton input.lhe output.dat
```

`output.dat` is optional. If it is not supplied, the output will be displayed in the screen.

It can also be installed by

```
make install DESTDIR=/some/where
```

Setting `PATH` or `LD_LIBRARY_PATH` is not necessary for using this. See the `install` target of [`Makefile`](Makefile).
