#include <stdio.h>
#include <math.h>
#include <float.h>

static double BALL_MASS = 1;
static double GRAVITY = 9.8;
static double TABLE_FRICTION_K = 0.1;
static const double EPS = 1e-12;

enum states {SHOOTING, MOVING};

enum states state = SHOOTING;

/* clamp small values to zero */
static double near_zero(double v) {
    return fabs(v) < 1e-12 ? 0.0 : v;
}

double sgn(double val) {
    return (0.0 < val) - (val < 0.0);
}

/* returns velocity at time 't' for a single segment with initial velocity vi */
double pt_segment(double t, double vi) {
    vi = near_zero(vi);

    double friction_force = TABLE_FRICTION_K * BALL_MASS * GRAVITY;
    double a = (-sgn(vi) * friction_force) / BALL_MASS; // always opposes motion

    double t_stop = -vi / a; // positive if a opposes vi
    if(t < t_stop){
        return (vi * t) + ((a * (t*t))/2);
    }
    return 0.0;
}

int main(){
    enum states current_state = SHOOTING;
    double vi_x = 1;
    double last = 0;
    
    for(int i=0; i<1000; i++){
        double t = (double)i/100;
        double x = pt_segment(t, vi_x);
        printf("t: %f X: %f dx: %f\n", t, x, x - last);
        last = x;
    }

    return 0;
}