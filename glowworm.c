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




void glowworm_optimizer(double(*fitnessfunction)(double*), 
        unsigned int dim, double ftarget, double eval_budget)
{
    int i, j;
    double f;
    struct GlowWorm gw[GLOW_COUNT];

    eval_budget = fmin(1000000000. * dim, eval_budget);
    rand_init_worms(gw, GLOW_COUNT, dim);

    /* keep track of budget better */
    for (i = 0.; i < eval_budget; i++)
    {
        /*phase 1: update luciferin*/
        for (j = 0; j < GLOW_COUNT; j++){
            f = fitnessfunction(gw[j].pos);
            if (f < ftarget)
                break;
            gw[j].l *= (1 - LUCI_DECAY);
            gw[j].l += LUCI_ENHANCE * f;
        }
        /*phase 2: move*/












    }
}
