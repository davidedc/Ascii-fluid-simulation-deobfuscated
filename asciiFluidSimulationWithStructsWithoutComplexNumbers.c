// this de-obfuscated version by Davide Della Casa
// original obfuscated version by Yusuke Endoh
// compile like so:
// gcc asciiFluidSimulation.c -o asciiFluidSimulation

#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 24

int xSandboxAreaScan = 0, ySandboxAreaScan = 0;

struct Particle {
	double xPos;
	double yPos;
	double density;
	int wallflag;
	double xForce;
	double yForce;
	double xVelocity;
	double yVelocity;
} particles[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2]; 

double xParticleDistance, yParticleDistance;
double particlesInteraction;
double particlesDistance;

int	x, y, screenBufferIndex, totalOfParticles;
int	gravity = 1, pressure = 4, viscosity = 7;

char screenBuffer[CONSOLE_WIDTH * CONSOLE_HEIGHT + 1];


int main(){

    // terminal escape code to clear the screen
    puts("\x1b[2J");

	// read the input file to initialise the particles.
    // # stands for "wall", i.e. unmovable particles (very high density)
    // any other non-space character represents normal particles.
	int particlesCounter = 0;
	while ((x = getc(stdin)) != EOF) {

		switch ( x ) {
			case '\n':
		        // next row
				// rewind the x to -1 cause it's gonna be incremented at the
				// end of the while body
		        ySandboxAreaScan += 2;
		        xSandboxAreaScan = -1;
			 	break;
			case ' ':
				break;
			case '#':
				// The character # represents “wall particle” (a particle with fixed position),
				// and any other non-space characters represent free particles.
				// A wall sets the flag on 2 particles side by side.
				particles[particlesCounter].wallflag = particles[particlesCounter + 1].wallflag = 1;
			default:
        		// Each non-empty character sets the position of two
        		// particles one below the other (real part is rows)
			    // i.e. each cell in the input file corresponds to 1x2 particle spaces,
			    // and each character sets two particles
			    // one on top of each other.
			    // It's as if the input map maps to a space that has twice the height, as if the vertical
			    // resolution was higher than the horizontal one.
			    // This is corrected later, see "y scale correction" comment.
			    // I think this is because of gravity simulation, the vertical resolution has to be
			    // higher, or conversely you can get away with simulating a lot less of what goes on in the
			    // horizontal axis.
        		particles[particlesCounter].xPos = xSandboxAreaScan;
        		particles[particlesCounter].yPos = ySandboxAreaScan;

        		particles[particlesCounter + 1].xPos = xSandboxAreaScan;
        		particles[particlesCounter + 1].yPos = ySandboxAreaScan + 1;

				// we just added two particles
				totalOfParticles = particlesCounter += 2;


		}
        // next column
        xSandboxAreaScan += 1;
 
    }

    while (1) { 

		int particlesCursor, particlesCursor2;

        // Iterate over every pair of particles to calculate the densities
		for (particlesCursor = 0; particlesCursor < totalOfParticles; particlesCursor++){
			// density of "wall" particles is high, other particles will bounce off them.
			particles[particlesCursor].density = particles[particlesCursor].wallflag * 9;

			for (particlesCursor2 = 0; particlesCursor2 < totalOfParticles; particlesCursor2++){

				xParticleDistance = particles[particlesCursor].xPos - particles[particlesCursor2].xPos;
				yParticleDistance = particles[particlesCursor].yPos - particles[particlesCursor2].yPos;
				particlesDistance = sqrt( pow(xParticleDistance,2.0) + pow(yParticleDistance,2.0));
				particlesInteraction = particlesDistance / 2.0 - 1.0;

				// this line here with the alternative test
				// works much better visually but breaks simmetry with the
				// next block
				//if (round(creal(particlesInteraction)) < 1){
				// density is updated only if particles are close enough
				if (floor(1.0 - particlesInteraction) > 0){
					particles[particlesCursor].density += particlesInteraction * particlesInteraction;
                }
            }
        }

        // Iterate over every pair of particles to calculate the forces
		for (particlesCursor = 0; particlesCursor < totalOfParticles; particlesCursor++){
			particles[particlesCursor].yForce = gravity;
			particles[particlesCursor].xForce = 0;

			for (particlesCursor2 = 0; particlesCursor2 < totalOfParticles; particlesCursor2++){

				xParticleDistance = particles[particlesCursor].xPos - particles[particlesCursor2].xPos;
				yParticleDistance = particles[particlesCursor].yPos - particles[particlesCursor2].yPos;
				particlesDistance = sqrt( pow(xParticleDistance,2.0) + pow(yParticleDistance,2.0));
				particlesInteraction = particlesDistance / 2.0 - 1.0;

				// force is updated only if particles are close enough
				if (floor(1.0 - particlesInteraction) > 0){
					particles[particlesCursor].xForce += particlesInteraction * (xParticleDistance * (3 - particles[particlesCursor].density - particles[particlesCursor2].density) * pressure + particles[particlesCursor].xVelocity *
					  viscosity - particles[particlesCursor2].xVelocity * viscosity) / particles[particlesCursor].density;
					particles[particlesCursor].yForce += particlesInteraction * (yParticleDistance * (3 - particles[particlesCursor].density - particles[particlesCursor2].density) * pressure + particles[particlesCursor].yVelocity *
					  viscosity - particles[particlesCursor2].yVelocity * viscosity) / particles[particlesCursor].density;
                }
            }
        }


		// empty the buffer
		for (screenBufferIndex = 0; screenBufferIndex < CONSOLE_WIDTH * CONSOLE_HEIGHT; screenBufferIndex++){
			screenBuffer[screenBufferIndex] = 0;
        }


		for (particlesCursor = 0; particlesCursor < totalOfParticles; particlesCursor++) {

			if (!particles[particlesCursor].wallflag) {
				
				// This is the newtonian mechanics part: knowing the force vector acting on each
				// particle, we accelerate the particle (see the change in velocity).
				// In turn, velocity changes the position at each tick.
				// Position is the integral of velocity, velocity is the integral of acceleration and
				// acceleration is proportional to the force.

				// force affects velocity
				if ( sqrt( pow(particles[particlesCursor].xForce ,2.0) + pow(particles[particlesCursor].yForce,2.0)) < 4.2) {
					particles[particlesCursor].xVelocity += particles[particlesCursor].xForce / 10;
					particles[particlesCursor].yVelocity += particles[particlesCursor].yForce / 10;
				}
				else {
					particles[particlesCursor].xVelocity += particles[particlesCursor].xForce / 11;
					particles[particlesCursor].yVelocity += particles[particlesCursor].yForce / 11;
				}

				// velocity affects position
				particles[particlesCursor].xPos += particles[particlesCursor].xVelocity;
				particles[particlesCursor].yPos += particles[particlesCursor].yVelocity;
			}


			// given the position of the particle, determine the screen buffer
			// position that it's going to be in.
			x = particles[particlesCursor].xPos;
			// y scale correction, since each cell of the input map has
			// "2" rows in the particle space.
			y = particles[particlesCursor].yPos/2;
			screenBufferIndex = x + CONSOLE_WIDTH * y;


			// if the particle is on screen, update
			// four buffer cells around it
			// in a manner of a "gradient",
			// the representation of 1 particle will be like this:
			//
			//      8 4
			//      2 1
			//
			// which after the lookup that puts chars on the
			// screen will look like:
			//
			//      ,.
			//      `'
			//
			// With this mechanism, each particle creates
			// a gradient over a small area (four screen locations).
			// As the gradients of several particles "mix",
			// (because the bits are flipped
			// independently),
			// a character will be chosen such that
			// it gives an idea of what's going on under it.
			// You can see how corners can only have values of 8,4,2,1
			// which will have suitably "pointy" characters.
			// A "long vertical edge" (imagine two particles above another)
			// would be like this:
			//
			//      8  4
			//      10 5
			//      2  1
			//
			// and hence 5 and 10 are both vertical bars.
			// Same for horizontal edges (two particles aside each other)
			//
			//      8  12 4
			//      2  3  1
			//
			// and hence 3 and 12 are both horizontal dashes.
			// ... and so on for the other combinations such as
			// particles placed diagonally, where the diagonal bars
			// are used, and places where four particles are present,
			// in which case the highest number is reached, 15, which
			// maps into the blackest character of the sequence, '#'

			if (y >= 0 && y < CONSOLE_HEIGHT - 1 && x >= 0 && x < CONSOLE_WIDTH - 1) {
				screenBuffer[screenBufferIndex]   |= 8; // set 4th bit to 1
				screenBuffer[screenBufferIndex+1] |= 4; // set 3rd bit to 1
				// now the cell in row below
				screenBuffer[screenBufferIndex + CONSOLE_WIDTH]   |= 2; // set 2nd bit to 1
				screenBuffer[screenBufferIndex + CONSOLE_WIDTH + 1] |= 1; // set 1st bit to 1
			}
			
        }

		// Update the screen buffer
		for (screenBufferIndex = 0; screenBufferIndex < CONSOLE_WIDTH * CONSOLE_HEIGHT; screenBufferIndex++){
			if ( screenBufferIndex % CONSOLE_WIDTH == CONSOLE_WIDTH - 1)
				screenBuffer[screenBufferIndex] = '\n';
			else
				// the string below contains 16 characters, which is for all
				// the possible combinations of values in the screenbuffer since
				// it can be subject to flipping of the first 4 bits
				screenBuffer[screenBufferIndex] = " '`-.|//,\\|\\_\\/#"[screenBuffer[screenBufferIndex]];
				// ---------------------- the mappings --------------
				// 0  maps into space
				// 1  maps into '    2  maps into `    3  maps into -
				// 4  maps into .    5  maps into |    6  maps into /
				// 7  maps into /    8  maps into ,    9  maps into \
				// 10 maps into |    11 maps into \    12 maps into _
				// 13 maps into \    14 maps into /    15 maps into #
        }

        // terminal escape code to put cursor back to the top left of the screen
        puts("\x1b[1;1H");
        // finally blit the screen buffer to screen
        puts(screenBuffer);

        // don't peg the cpu, be merciful, pause a little.
		usleep(3000);
	} 
    
}