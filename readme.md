# What

The best de-obfuscated versions of Yusuke Endoh's "Most complex ASCII fluid" obfuscated C code competition 2012 entry.
The original source is [here](http://www.ioccc.org/2012/endoh1/hint.html). There are a couple of other de-obfuscated versions online, but they only basically de-macro and break down some constructs.

These ones go the full way. There are three versions:

1. "asciiFluidSimulation". Preserves the "simgle array of complex numbers" approach. Removes all pointer arithmetics and provides extended commentary on the tecnique used to calculate and render the fluid simulation.

2. "asciiFluidSimulationWithoutComplexNumbers". Same as 1 but also removes all use of complex numbers, just uses standard doubles instead (using complex numbers for vectors is clever and compact but convoluted), and separate arrays are used for each data field of particles.

3. "asciiFluidSimulationWithStructsWithoutComplexNumbers". Same as 2 but particles data are now cleanly put into a struct.

Also see [a browser-based implementation here.](https://github.com/davidedc/Basic-fluid-simulation-in-the-browser)

# How to run
```gcc asciiFluidSimulation.c -o asciiFluidSimulation```

then run it with one of the provied example .txt files (most of them from Yusuke Endoh) like so:

```./asciiFluidSimulation < ./examples/terraces.txt```

P.S. add the ```-O3``` flag when compiling (```gcc -O3 ...```) to really speed up the simulation.

# Examples

Column:
<p align="center">
  <img src="https://raw.githubusercontent.com/davidedc/Ascii-fluid-simulation-deobfuscated/master/readme-images/column.gif">
</p>

Terraces:
<p align="center">
  <img src="https://raw.githubusercontent.com/davidedc/Ascii-fluid-simulation-deobfuscated/master/readme-images/terraces.gif">
</p>

Pour-out:
<p align="center">
  <img src="https://raw.githubusercontent.com/davidedc/Ascii-fluid-simulation-deobfuscated/master/readme-images/pour-out.gif">
</p>

Clock:
<p align="center">
  <img src="https://raw.githubusercontent.com/davidedc/Ascii-fluid-simulation-deobfuscated/master/readme-images/clock.gif">
</p>

# How does it work?

...it uses a version of the "smoothed-particle hydrodynamics" (SPC) method.

Briefly: each particle has an associated velocity vector. At each step, the system calculates the "density" of each particicle (a scalar). Then it calculates the total force (due to gravity, the distance from and the density of all the other particles). Finally, it modifies the velocity of each particle due to the calculated resulting force, and updates its position according to the velocity.

More in depth:

...it first calculates the "density" of each particle, which is just a number (a scalar). The density of each particle is calculated by checking how close it is to each other particles, and it gives a sense of how "compressed" the particle is. For example a particle in the middle of a cluster of particles has high density, while a particle on the edge has low density. The density is useful because it measures how much a particle is free to move. So a force applied to a low-density particle will move it much more than the same force applied to a high-density particle.

Next, it calculates the actual force to be applied to each particle. The total force on a particle is a vector that adds the gravity to a repulsion force from each other particle (particles tend to "spread away" from each other). The force between two particles is affected by their distance and their densities.

Then, it calculates the new velocity and the new position of each particle based on its force vector.
