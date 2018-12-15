#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define VIS_FILE "vis_data.txt"
#define GLOW_COUNT 10
#define MAX_DIM 40
#define BUF_SIZE 5000


#define RHO 0.4
#define GAMMA 0.6
#define BETA 0.08
#define N_T 5
#define STEP_SIZE 0.03
#define L_0 5
/* These must be the same */
#define R_S 1.0 
#define R_0 1.0



struct Glowworm{
    double pos[MAX_DIM];
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
        gw[i].l = L_0;
        gw[i].r = R_0;
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


double calc_radius(struct Glowworm gw1, int k){
    double res;
    res = fmax(0, gw1.r + BETA * (N_T - k));
    res = fmin(R_S, res);
    return res;
}



// reverses a string 'str' of length 'len' 
static void reverse(char *str, int len){ 
	int i = 0, j = len - 1, temp; 
	while (i < j){ 
		temp = str[i]; 
		str[i] = str[j]; 
		str[j] = temp; 
		i++; j--; 
	} 
} 


// Converts a given integer x to string str[].  d is the number 
// of digits required in output. If d is more than the number 
// of digits in x, then 0s are added at the beginning. 
static int intToStr(int x, char str[], int d){ 
	int i = 0; 
	while (x){ 
		str[i++] = (x%10) + '0'; 
		x = x/10; 
	} 
		  
	// If number of digits required is more, then 
	// add 0s at the beginning 
	while (i < d) 
		str[i++] = '0'; 
	reverse(str, i); 
	str[i] = '\0'; 
	return i; 
}



// Converts a floating point number to string. 
static void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
}


void visualize(FILE *fp, struct Glowworm *gw, unsigned int dim){
    char *t1, *t2, *t3;
    int i, j;
    double d;
    t1 = malloc(BUF_SIZE * sizeof(char));
    t2 = malloc(BUF_SIZE * sizeof(char));
    t3 = malloc(BUF_SIZE * sizeof(char));
    for (i = 0; i < GLOW_COUNT; i++){
        t1[0] = '\0';
        t2[0] = '\0';
        t3[0] = '\0';
		sprintf(t3, "%i, ", i);
        for (j = 0; j < dim; j++){
            d = gw[i].pos[j];
			ftoa(fabs(d), t1, 5);
			if (d <= -1)
				sprintf(t2, "-%s", t1);
			else if (d < 0)
				sprintf(t2, "-0%s", t1);
			else if (d >= 1)
				sprintf(t2, "%s", t1);
			else
				sprintf(t2, "0%s", t1);
            strcat(t3, t2);
            if (j < dim - 1)
                strcat(t3, ", ");

        }
        fprintf(fp, "%s\n", t3);
    }

}


void glowworm_visualizer(double(*fitnessfunction)(double*), 
        unsigned int dim, double ftarget, double eval_budget){
    int i, j, k, gw_s;
    double evals = 0.;
    double f, t;
    FILE *fp;
    struct Glowworm *gw = malloc(sizeof(struct Glowworm) * GLOW_COUNT);
    struct Glowworm *gw_new;
    int cw[GLOW_COUNT]; /* candidate worms */

    eval_budget = fmin(1000000000. * dim, eval_budget);
    rand_init_worms(gw, GLOW_COUNT, dim);

    fp = fopen(VIS_FILE, "w");

    t = 0;
    while(1) {
        visualize(fp, gw, dim);
        /*phase 1: update luciferin*/
        for (i = 0; i < GLOW_COUNT; i++){
            f = fitnessfunction(gw[i].pos);
            evals++;
            if (f < ftarget || evals > eval_budget){
                free(gw);
                return;
            }
            gw[i].l = (1.0 - RHO) * gw[i].l + GAMMA * f;
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
            gw_new[i].r = calc_radius(gw_new[i], k);
        }
        /* swap to new generation of worms */
        free(gw);
        gw = gw_new;
        t++;
    }
    fclose(fp);
}
