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
