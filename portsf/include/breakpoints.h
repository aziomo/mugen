#include <stdio.h>

typedef struct {
	double time;
	double value;
} BREAKPOINT;

BREAKPOINT maxpoint(const BREAKPOINT* points, long npoints);
BREAKPOINT* get_breakpoints(FILE* fp, long* psize);
int inrange(const BREAKPOINT* points, double minval, double maxval, unsigned long npoints);
double val_at_brktime(const BREAKPOINT* points, unsigned long size, double sampletime);
