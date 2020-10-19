# pt-sim

Tests and resources for the PT-sim project.


The files `pt-sim.sh` and `pt-sim-aging.sh` are wrapper files you should modify to call your executables.

You should add Makefile logic so that your program can be run with commands like:

    $ make ptsim
    $ make ptsim-aging
    $ sh ptsim test/aPageTableFile < test/anInputFile
    $ sh ptsim-aging test/aPageTableFile < test/anInputFile
