#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <SDL3/SDL.h>

SDL_Window* win;
SDL_Renderer* ren;

static double BALL_MASS = 0.160;
static double GRAVITY = 9.8;
static double TABLE_FRICTION_K = 0.015;
static const double EPS = 1e-12;

enum states {SHOOTING, MOVING};

typedef struct {
    double start_time;
    double end_time;
    double pix, piy;
    double vix, viy;
} MotionSegment;

typedef struct {
    MotionSegment *data;
    int count;
    int capacity;
} SegmentList;

SegmentList *init_segment_list() {
    SegmentList *list = malloc(sizeof(SegmentList));
    if(!list) return NULL;
    list->capacity = 4;
    list->count = 0;
    list->data = malloc(sizeof(MotionSegment) * list->capacity);
    if (!list->data) { free(list); return NULL; }
    return list;
}

void add_segment(SegmentList *list, MotionSegment seg) {

    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, sizeof(MotionSegment) * list->capacity);
    }

    list->data[list->count] = seg;
    list->count++;
}

/* clamp small values to zero */
static double near_zero(double v) {
    return fabs(v) < 1e-12 ? 0.0 : v;
}

double sgn(double val) {
    return (0.0 < val) - (val < 0.0);
}

//returns position given t = time, vi = initial velocity, pi = initial position.
double p_of_t(double t, double vi, double pi, double friction_force) {
    vi = near_zero(vi);

    double a = (-sgn(vi) * friction_force) / BALL_MASS; // always opposes motion

    double t_stop = -vi / a; // positive if a opposes vi
    if(t < t_stop){
        return pi + (vi * t) + ((a * (t*t))/2);
    }
}

//returns time given at a target position (or 0 if not found) given friction_force, v = velocity, pi = initial position, p = target position
double t_of_x(double friction_force, double v, double pi, double p){
    double a = (-sgn(v) * friction_force) / BALL_MASS;
    double plus = (-v + sqrtf(pow(v, 2) - 2*(a)*(pi-p)))/a;
    double minus = (-v - sqrtf(pow(v, 2) - 2*(a)*(pi-p)))/a;
    if(plus >= 0) return plus;
    if(minus >= 0) return minus;
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

    SDL_Delay(10);
    return true;
}

bool draw_ball(double x, double y){
    SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ren);

    SDL_FRect rect;
    rect.x = x*20;
    rect.y = y;
    rect.w = rect.h = 20;
    SDL_SetRenderDrawColor(ren, 0, 156, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(ren, &rect);

    SDL_RenderPresent(ren);
    return true;
}
int main(){
    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("SDl3 Test", 640, 480, 0);
    ren = SDL_CreateRenderer(win, NULL);

    double friction_force = TABLE_FRICTION_K * BALL_MASS * GRAVITY;

    enum states current_state = SHOOTING;
    double vix = 5;
    double last = 0;
    

    SDL_Delay(3000);

    SegmentList * seglist = init_segment_list();
    MotionSegment initial_segment = {.pix = 0, .piy = 0, .vix = vix, .viy = 0, .start_time = 0};
    int step = 0;
    add_segment(seglist, initial_segment);

    for(int i = 0; i<seglist->count; i++){
        printf("%f\n", seglist->data[i].vix);
    }
    /*
    int run = 1;
    while(run){
        
    }
    */
    
    /*
    for(int i=0; i<1000; i++){
        double t = (double)i/100;
        double x = p_of_t(t, vix, 0, friction_force);
        printf("t: %f X: %f dx: %f\n", t, x, x - last);
        last = x;

        draw_ball(x,0);

        SDL_Delay(10);
    }
    */

    //close out fam
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}