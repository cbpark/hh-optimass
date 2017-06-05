# hh-optimass

## Build prerequisites

* C++ compiler supporting C++11 features ([Clang](http://clang.llvm.org/cxx_status.html) >= 3.3, [GCC](https://gcc.gnu.org/projects/cxx-status.html) >= 4.8.1),
* [CLHEF](https://github.com/cbpark/CLHEF),
* [ROOT](https://root.cern.ch),
* OptiMass/alm_base.

Adjust `CLHEF` and `OPTIMASS` to their installation path in [`Makefile`](Makefile). For instance,

```
CLHEF=/clhef/path OPTIMASS=/optimass/path make
```

and check if `root-config` is in your `PATH`.
