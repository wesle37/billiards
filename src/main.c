#include <stdio.h>
#include <math.h>
#include <float.h>
#include <SDL3/SDL.h>

SDL_Window* win;
SDL_Renderer* ren;

static double BALL_MASS = 1;
static double GRAVITY = 9.8;
static double TABLE_FRICTION_K = 0.1;
static const double EPS = 1e-12;

enum states {SHOOTING, MOVING};

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
bool update(){
    SDL_Event e;
    if(SDL_PollEvent(&e)){
        if(e.type == SDL_EVENT_QUIT){
            return false;
        }
        if(e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE){
            return false;
        }
    }
    
    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren);

    SDL_FRect rect;
    rect.x = rect.y = 100;
    rect.w = rect.h = 100;
    SDL_SetRenderDrawColor(ren, 0, 156, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(ren, &rect);

    SDL_RenderPresent(ren);

    SDL_Delay(1);
    
}
int main(){
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("SDl3 Test", 640, 480, 0);
    ren = SDL_CreateRenderer(win, NULL);

    enum states current_state = SHOOTING;
    double vi_x = 5;
    double last = 0;

    SDL_Delay(3000);
    
    for(int i=0; i<500; i++){
        double t = (double)i/100;
        double x = pt_segment(t, vi_x);
        printf("t: %f X: %f dx: %f\n", t, x, x - last);
        last = x;

        SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(ren);

        SDL_FRect rect;
        rect.x = x*20;
        rect.y = 0;
        rect.w = rect.h = 20;
        SDL_SetRenderDrawColor(ren, 0, 156, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(ren, &rect);

        SDL_RenderPresent(ren);

        SDL_Delay(10);
    }

    //update
    bool run = true;
    /*
    while(run){
        run = update();
    }*/

    //close out fam
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}