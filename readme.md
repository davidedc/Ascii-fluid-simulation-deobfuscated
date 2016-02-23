# What

The best de-obfuscated version of Yusuke Endoh's "Most complex ASCII fluid" obfuscated-code competition 2012 entry.
The original source is here: http://www.ioccc.org/2012/endoh1/hint.html . There are a couple of other de-obfuscated versions online, but they only basically de-macro and break down some constructs.

This one goes the full way (pointer arithmetic no more!) and provides extended commentary on the tecnique used to calculate and render the fluid simulation.

# How to run
```gcc asciiFluidSimulation.c -o asciiFluidSimulation```
then run it with one of the .txt files like so
```./asciiFluidSimulation < terraces.txt```

# Example of what it looks like

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




