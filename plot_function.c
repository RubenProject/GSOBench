/* runs an entire experiment for benchmarking 
* on the noise-free testbed
* or the noisy testbed (change the ifun loop in this case as given below).
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "bbobStructures.h" /* Include all declarations for BBOB calls */

/* include all declarations for your own optimizer here */
void glowworm_optimizer(double(*fitnessfunction)(double*), unsigned int dim, double ftarget,
                  double maxfunevals);

int main(void)
{
    unsigned int seed;
    double maxfunevals, minfunevals;

    ParamStruct params = fgeneric_getDefaultPARAMS();

    /* modify the following parameters, choosing a different setting
     * for each new experiment */
    strcpy(params.dataPath, "trash");  /* different folder for each experiment! */
    /* please beforehand run from the command-line 'python createfolders.py PUT_MY_BBOB_DATA_PATH'
     * to create the necessary folder structure to run an experiment. */
    strcpy(params.algName, "glowworm");
    strcpy(params.comments, "just for visualization purposes");


    seed = 42;
    srand(seed); /* used by optimization function*/
    printf("random seed set to %d\n", seed);

    params.DIM = 2;
    params.funcId = 1;
    params.instanceId = 1;
    /* call the BBOB initialization */
    fgeneric_initialize(params);

    maxfunevals = 1.0 * 10000 * 2; /* PUT APPROPRIATE MAX. NUMBER OF FEVALS */
                                     /* 5. * dim should be fine to just check everything */
    minfunevals = 2 + 2;  /* PUT MINIMAL USEFUL NUMBER OF FEVALS */

	double i, j, res;
	double mick[2];
	for (i = -5; i < 5; i += 0.2){
		for (j = -5; j < 5; j += 0.2){
			mick[0] = i;
			mick[1] = j;
			res = fgeneric_evaluate(mick);
			printf("%f, %f, %f\n", i, j, res);
		}
	}



    fgeneric_finalize();
    return 0;
}
