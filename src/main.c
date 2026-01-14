#include <stdio.h>
#include <math.h>
#include <float.h>

static double BALL_MASS = 1;
static double GRAVITY = 9.8;
static double TABLE_FRICTION_K = 0.1;
static const double EPS = 1e-12;

static enum states {SHOOTING};

/* clamp small values to zero */
static double near_zero(double v) {
    return fabs(v) < 1e-12 ? 0.0 : v;
}

double sgn(double val) {
    return (0.0 < val) - (val < 0.0);
}

/* returns velocity at time 't' for a single segment with initial velocity vi */
double velo_segment(double t, double vi) {
    vi = near_zero(vi);
    if (vi == 0.0) return 0.0;

    double friction_force = TABLE_FRICTION_K * BALL_MASS * GRAVITY;
    double a = -sgn(vi) * (friction_force / BALL_MASS); // always opposes motion
    if (a == 0.0) return vi;

    double t_stop = -vi / a; // positive if a opposes vi
    if (t >= t_stop) return 0.0;
    return vi + a * t;
}

double ball_velo_x(double time, double vi){
    double friction = (TABLE_FRICTION_K * BALL_MASS * GRAVITY * -1.0 * sgn(vi));
    if(vi + ((friction / BALL_MASS) * time) <= 0){
        friction = 0.0;
    }
    double a = (friction)/BALL_MASS;
    double v = vi + a*time;
    return v;
}

double integrate_simpson(double (*f)(double, double), double vi, double a, double b, int n) {
    if (a == b) return 0.0;
    if (n <= 0) n = 1000;        /* default */
    if (n % 2 == 1) n++;         /* make even */

    double h = (b - a) / n;
    double s = f(a, vi) + f(b, vi);

    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        s += (i % 2 == 0) ? 2.0 * f(x, vi) : 4.0 * f(x, vi);
    }
    return s * h / 3.0;
}

int main(){
    enum states current_state = SHOOTING;
    double vi_x = 1;
    double x = integrate_simpson(ball_velo_x, vi_x, 0, 1, 1000);
    printf("X position after 1 second: %f\n", x);

    for(int i=0; i<1000; i++){
        printf("t: %f X: %f\n", (double)i/100, integrate_simpson(ball_velo_x, vi_x, 0, (double)i/100, 1000));
    }
    return 0;
}