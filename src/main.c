#include <stdio.h>
#include <math.h>

static double BALL_MASS = 1;
static double GRAVITY = 9.8;
static double TABLE_FRICTION_K = 0.1;
static enum states {SHOOTING};

double ball_velo_x(double time, double vi){
    double res = vi - (TABLE_FRICTION_K * GRAVITY * time);
    res = res > 0.0 ? res : 0.0;
    return res;
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

    for(int i=0; i<100; i++){
        printf("X: %f\n", integrate_simpson(ball_velo_x, vi_x, 0, (double)i/100, 1000));
    }
    return 0;
}