#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

#define GLOW_COUNT 100
#define RHO 0.4
#define GAMMA 0.6
#define BETA 0.08
#define NT 5
#define STEP_SIZE 0.03
#define LUCI_0 5



struct Glowworm{
    double pos[40];
    int l;/*luciferin*/
    int r;/*radius*/
};


double euclid_dist(struct Glowworm gw1, struct Glowworm gw2, unsigned int dim){
    unsigned int i = 0;
    double dist = 0.;
    for (i = 0; i < dim; i++){
        dist += abs(gw1.pos[i] - gw2.pos[i]);
    }
    return dist;
}


void rand_init_worms(struct Glowworm *gw, unsigned int n, unsigned int m){
    unsigned int i, j;
    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++){
            gw[i].pos[j] = 10. * ((double)rand() / RAND_MAX) - 5.;
        }
        gw[i].l = 0;
        gw[i].r = 0;
    }
}

/* calculate probabilities for all candidates of gw[i] */
/* and select */
int select_worm(struct Glowworm gw1, struct Glowworm *gw, int *cw, int n){
    int i;
    double t, sum;
    double p[GLOW_COUNT];
    sum = 0;
    for (i = 0; i < n; i++){
        sum += gw[cw[i]].l - gw1.l;
    }
    for (i = 0; i < n; i++){
        p[i] = (gw[cw[i]].l - gw1.l) / sum;
    }

    /* random number between 0 and 1 */
    t = rand() % 10000 / 10000.0;
    for (i = 0; i < n; i++){
        if (t < p[i])
            return i;
        else
            t -= p[i];
    }
    return -1;
}


struct Glowworm move_worm(struct Glowworm gw1, struct Glowworm gw2, unsigned int dim){
    struct Glowworm gw3;
    /* something something euclidean norm operator ? */
    euclid_dist(gw1, gw2, dim);
    return gw3;
}


void update_radius(struct Glowworm *gw1, struct Glowworm *gw, unsigned int dim){
}


void glowworm_optimizer(double(*fitnessfunction)(double*), 
        unsigned int dim, double ftarget, double eval_budget){
    int i, j, k, gw_s;
    double evals = 0.;
    double f, t;
    struct Glowworm *gw = malloc(sizeof(struct Glowworm) * GLOW_COUNT);
    struct Glowworm *gw_new;
    int cw[GLOW_COUNT]; /* candidate worms */

    srand(42);
    eval_budget = fmin(1000000000. * dim, eval_budget);
    rand_init_worms(gw, GLOW_COUNT, dim);

    t = 0;
    while(1) {
        /*phase 1: update luciferin*/
        for (i = 0; i < GLOW_COUNT; i++){
            f = fitnessfunction(gw[i].pos);
            evals++;
            if (f < ftarget)
                break;
            gw[i].l = (1 - RHO) * gw[i].l + GAMMA * f;
        }
        /*phase 2: move*/
        gw_new = malloc(sizeof(struct Glowworm) * GLOW_COUNT);
        /* this part should be a new function */
        for (i = 0; i < GLOW_COUNT; i++){
            k = 0;
            for (j = 0; j < GLOW_COUNT; j++){
                if (i != j 
                   && gw[i].l < gw[j].l
                   && gw[i].r >= euclid_dist(gw[i], gw[j], dim)){
                    /* add it to some candidate set for gw[i]*/
                    cw[k++] = j;
                }
            }
            /* select */
            gw_s = select_worm(gw[i], gw, cw, k);
            /* move towards selected candidate */
            gw_new[i] = move_worm(gw[i], gw[gw_s], dim);
            /* update radius */
            update_radius(&gw_new[i], gw, dim);

        }
        /* swap to new generation of worms */
        free(gw);
        gw = gw_new;
        t++;
    }
}
