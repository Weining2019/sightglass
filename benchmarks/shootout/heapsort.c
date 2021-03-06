
#include <sightglass.h>

//#include <math.h>
#include <stdlib.h>

#define LENGTH 10000
#define ITERATIONS 2000

#define IM 139968
#define IA 3877
#define IC 29573

static double
gen_random(double max)
{
    static long last = 42;
    return max * (last = (last * IA + IC) % IM) / IM;
}

static void
my_heapsort(int n, double *ra)
{
    int    i, j;
    int    ir = n;
    int    l  = (n >> 1) + 1;
    double rra;

    for (;;) {
        if (l > 1) {
            rra = ra[--l];
        } else {
            rra    = ra[ir];
            ra[ir] = ra[1];
            if (--ir == 1) {
                ra[1] = rra;
                return;
            }
        }

        i = l;
        j = l << 1;
        while (j <= ir) {
            if (j < ir && ra[j] < ra[j + 1]) {
                ++j;
            }
            if (rra < ra[j]) {
                ra[i] = ra[j];
                j += (i = j);
            } else {
                j = ir + 1;
            }
        }
        ra[i] = rra;
    }
}

typedef struct HeapSortCtx_ {
    int     n;
    double *ary;
    double  res;
} HeapSortCtx;

void
heapsort_setup(void *global_ctx, void **ctx_p)
{
    (void) global_ctx;

    static HeapSortCtx ctx;
    ctx.n  = LENGTH;
    *ctx_p = (void *) &ctx;
}

static double buf[LENGTH + 1] = { 0 };
void
heapsort_body(void *ctx_)
{
    HeapSortCtx *ctx = (HeapSortCtx *) ctx_;

    int i, j;
    //ctx->ary = calloc(ctx->n + 1, sizeof *ctx->ary);
    ctx->ary = buf;
    for (i = 0; i < ITERATIONS; i++) {
        BLACK_BOX(ctx->ary);
        BLACK_BOX(ctx->n);
        for (j = 1; j <= ctx->n; j++) {
            ctx->ary[j] = gen_random(1.0);
        }
        my_heapsort(ctx->n, ctx->ary);
        ctx->res = ctx->ary[ctx->n];
        BLACK_BOX(ctx->res);
    }
    //free(ctx->ary);
}
