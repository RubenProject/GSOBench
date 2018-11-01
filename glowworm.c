#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>


#define GLOW_COUNT 100
#define STEP_SIZE 1
#define RADIUS_ZERO 10
#define LUCI_DECAY 0.1
#define LUCI_ENHANCE 0.2

struct GlowWorm{
    double pos[40];
    int l;/*luciferin*/
    int r;/*radius*/
};


void rand_init_worms(struct GlowWorm *gw, 
        const unsigned int n, const unsigned int m)
{
    unsigned int i, j;
    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){
            gw[i].pos[j] = 10. * ((double)rand() / RAND_MAX) - 5.;
        }
        gw[i].l = 0;
        gw[i].r = RADIUS_ZERO;
    }
}

double euclid_dist(struct Glowworm gw1, struct GlowWorm gw2, unsigned int dim){
    double dist = 0.;
    for (unsigned int i = 0; i < dim; i++){
        dist += abs(gw1.pos[i] - gw2.pos[i]);
    }
    return dist;
}


void glowworm_optimizer(double(*fitnessfunction)(double*), 
        unsigned int dim, double ftarget, double eval_budget)
{
    int i, j, k;
    double evals = 0.;
    double f, t;
    struct GlowWorm gw[GLOW_COUNT];

    eval_budget = fmin(1000000000. * dim, eval_budget);
    rand_init_worms(gw, GLOW_COUNT, dim);

    /* keep track of budget better */
    for (t = 0.; true; t++)
    {
        /*phase 1: update luciferin*/
        for (j = 0; j < GLOW_COUNT; j++){
            f = fitnessfunction(gw[j].pos);
            evals++;
            if (f < ftarget)
                break;
            gw[j].l *= (1 - LUCI_DECAY);
            gw[j].l += LUCI_ENHANCE * f;
        }
        /*phase 2: move*/
        /* this part should be a new function */
        for (j = 0; j < GLOW_COUNT; j++){
            for (k = 0; k < GLOW_COUNT; k++){
                if (j != k 
                   && gw[j].l < gw[k].l
                   && gw[j].r >= euclid_dist(gw[j], gw[k], dim)){
                    /* add it to some candidate set for gw[j]*/
                }
            }
            /* calculate probabilities for all candidates of gw[j] */
            /* select */
            /* move towards selected candidate */
            /* update radius */

        }
        












    }
}
