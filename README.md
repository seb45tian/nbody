# N-body simulation program

This program simulates the behaviour of a system of n particles/bodies following
[Newton's law of universal gravitation](https://en.wikipedia.org/wiki/Newton%27s_law_of_universal_gravitation "Newton").
The basic principles can be found [here](https://en.wikipedia.org/wiki/N-body_problem "N-body").

A number of random particles can be specified or  a file to read the particle data from. The format of the file has to be:

    [mass] [x] [y] [z] [vx] [vy] [vz]

You can choose between a brute-force approach (standard) or using the [Barnes-Hut method](https://en.wikipedia.org/wiki/Barnes–Hut_simulation "Barnes-Hut").


Use make to compile the code. Needs boost and GLUT libraries.


```
./nbody -h

Allowed options:
  -h [ --help ]                 produce help message
  -p [ --particles ] arg (=100) set number of particles
  -f [ --infile ] arg           specify input file
  -e [ --epsilon ] arg (=0)     set softening factor
  -t [ --theta ] arg (=1)       set threshold value for theta
  -B [ --Barneshut ] [=arg(=1)] use Barnes-Hut method
  -b [ --boundary ] [=arg(=1)]  use periodic boundary conditions
  -o [ --octants ] [=arg(=1)]   make octants visible
  -n [ --threads ] arg (=1)     specify number of threads (only for BH. No need
                                to add -B.)
```
Author
------
Sebastian Potthoff: <sebastian.potthoff@gmail.com>
