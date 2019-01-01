#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

#define MAX_GLOW 100
#define MAX_DIM 40


#define RHO 0.4
#define GAMMA 0.6
#define BETA 0.08
#define N_T 5
#define STEP_SIZE 0.03
#define L_0 5
/* These must be the same */
#define R_S 3.0
#define R_0 3.0



struct Glowworm{
    double pos[MAX_DIM];
    double l;/*luciferin*/
    double r;/*radius*/
    int s; /*glowworm hasn't moved for x iterations */
};


void print_worm(struct Glowworm gw, unsigned int dim){
    unsigned int i;
    for (i = 0; i < dim; i++){
        printf("%f, ", gw.pos[i]);
    }
    printf("\n");
}


double euclid_dist(struct Glowworm gw1, struct Glowworm gw2, unsigned int dim){
    unsigned int i = 0;
    double dist = 0.;
    for (i = 0; i < dim; i++){
        dist += fabs(gw1.pos[i] - gw2.pos[i]);
    }
    return dist;
}


void rand_init_worms(double(*fitnessfunction)(double*), struct Glowworm *gw,
                    unsigned int n, unsigned int m){
    unsigned int i, j, n_scout;
    double *f_scout;
    n_scout = n / 2;
    f_scout = malloc(n_scout * sizeof(double));
    //initialize scout flies
    for (i = 0; i < n_scout; i++){
        for (j = 0; j < m; j++){
            gw[i].pos[j] = 10. * ((double)rand() / RAND_MAX) - 5.;
        }
        gw[i].l = L_0;
        gw[i].r = R_0;
        gw[i].s = 0;
    }
    //evaluate scouts
    double f_total = 0;
    for (i = 0; i < n_scout; i++){
        f_scout[i] = 1 / fitnessfunction(gw[i].pos);
        f_total += f_scout[i];
    }
    
    for (i = 0; i < n_scout; i++){
        f_scout[i] /= f_total;
    }

    for (i = 1; i < n_scout; i++){
        f_scout[i - 1] += f_scout[i];
    }

    double r_val, radius, t;
    int parent = -1;
    //position of the rest of the worms is based of promising parent worms
    //selection of parent is done proportionally to their fitness
    radius = 1.0 / m;
    for (i = n_scout; i < n; i++){
        r_val = (double)rand() / RAND_MAX;
        parent = -1;
        for (j = 0; j < n_scout; j++){
            if (r_val < f_scout[j])
                parent = j;
        }
        for (j = 0; j < m; j++){
            t = gw[parent].pos[j] + radius * ((double)rand() / RAND_MAX) - radius / 2;
            t = fmin(t, 5.);
            t = fmax(t, -5.);
            gw[i].pos[j] = t;
        }
        gw[i].l = L_0;
        gw[i].r = R_0;
        gw[i].s = 0;
    }
    free(f_scout);
}

/* calculate probabilities for all candidates of gw[i] */
/* and select */
int select_worm(struct Glowworm gw1, struct Glowworm *gw, int *cw, int n){
    int i;
    double t, sum;
    double p[MAX_GLOW];
    sum = 0;
    for (i = 0; i < n; i++){
        sum += gw[cw[i]].l - gw1.l;
    }
    for (i = 0; i < n; i++){
        p[i] = (gw[cw[i]].l - gw1.l) / sum;
    }

    /* random number between 0 and 1 */
    t = (double)rand() / RAND_MAX;
    for (i = 0; i < n; i++){
        if (t < p[i])
            return cw[i];
        else
            t -= p[i];
    }
    return -1;
}


struct Glowworm move_worm(struct Glowworm gw1, struct Glowworm gw2, unsigned int dim){
    double t[MAX_DIM];
    double euclid_norm;
    unsigned int i;
    for (i = 0; i < dim; i++)
        t[i] = gw2.pos[i] - gw1.pos[i];

    euclid_norm = 0;
    for (i = 0; i < dim; i++)
        euclid_norm += t[i] * t[i];

    euclid_norm = sqrt(euclid_norm);
    for (i = 0; i < dim; i++)
        t[i] /= euclid_norm; 

    for (i = 0; i < dim; i++)
        gw1.pos[i] += STEP_SIZE * t[i];

    return gw1;
}


struct Glowworm copy_worm(struct Glowworm gw, unsigned int dim){
    struct Glowworm gw_new;
    unsigned int i;
    for (i = 0; i < dim; i++)
        gw_new.pos[i] = gw.pos[i];
    gw_new.r = gw.r;
    gw_new.l = gw.l;
    gw_new.s = gw.s;
    return gw_new;
}


double calc_radius(struct Glowworm gw, int k){
    double res;
    res = fmax(0., gw.r + BETA * (double)(N_T - k));
    res = fmin(R_S, res);
    return res;
}


int leader_worm(struct Glowworm *gw, struct Glowworm gw1, int n, unsigned int dim){
    int i, dist;
    for (i = 0; i < n; i++){
        dist = euclid_dist(gw1, gw[i], dim);
        if (dist > 0.01 && dist < R_S)
            return 1;
    }
    return 0;
}


struct Glowworm relocate_worm(struct Glowworm gw, unsigned int dim){
    unsigned int i;
    for (i = 0; i < dim; i++){
        gw.pos[i] = 10. * ((double)rand() / RAND_MAX) - 5.;
    }
    return gw;
}


void glowworm_optimizer(double(*fitnessfunction)(double*), 
        unsigned int dim, double ftarget, double eval_budget){
    int i, j, k, gw_s;
    double evals = 0.;
    double f;
    struct Glowworm *gw = malloc(sizeof(struct Glowworm) * MAX_GLOW);
    struct Glowworm *gw_new;
    int cw[MAX_GLOW]; /* candidate worms */
	int n = dim * 10;
    n = n > MAX_GLOW ? MAX_GLOW : n;
	

    eval_budget = fmin(1000000000. * dim, eval_budget);
    rand_init_worms(fitnessfunction, gw, n, dim);

    while(1) {
        /*phase 1: update luciferin*/
        for (i = 0; i < n; i++){
            f = fitnessfunction(gw[i].pos);
            evals++;
            if (f < ftarget || evals > eval_budget){
                free(gw);
                return;
            }
            gw[i].l = (1.0 - RHO) * gw[i].l + GAMMA * 1.0 / f;
        }
        /*phase 2: move*/
        gw_new = malloc(sizeof(struct Glowworm) * MAX_GLOW);
        /* this part should be a new function */
        for (i = 0; i < n; i++){
            k = 0;
            for (j = 0; j < n; j++){
                if (i != j 
                   && gw[i].l < gw[j].l
                   && gw[i].r >= euclid_dist(gw[i], gw[j], dim)){
                    /* add it to some candidate set for gw[i]*/
                    cw[k++] = j;
                }
            }
            if (k > 0){
                /* select */
                gw_s = select_worm(gw[i], gw, cw, k);
                /* move towards selected candidate */
                gw_new[i] = move_worm(gw[i], gw[gw_s], dim);
                gw_new[i].s = 0;
            } else {
                gw_new[i] = copy_worm(gw[i], dim);
                gw_new[i].s++;
                /* check to relocate */
                if (gw_new[i].s > 5 && gw_new[i].r == R_S){
                    if (!leader_worm(gw, gw_new[i], n, dim)){
                        gw_new[i] = relocate_worm(gw_new[i], dim);
                    }
                    gw_new[i].s = 0;
                }

            }
            /* update radius */
            gw_new[i].r = calc_radius(gw_new[i], k);
        }
        /* swap to new generation of worms */
        free(gw);
        gw = gw_new;
    }
}
