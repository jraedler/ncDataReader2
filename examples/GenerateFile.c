#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ncDataReader2.h"
#include "netcdf.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DIM     100
#define NPOINTS 1000

#define RAND01() (rand()/(double)RAND_MAX)

void handle_error(int status) {
if (status != NC_NOERR) {
   fprintf(stderr, "%s\n", nc_strerror(status));
   exit(-1);
   }
}


int createFile(char *fileName) {
    int ncFile;
    char c[100];
    float x = 42.42;
    int   y = 1234;
    handle_error(nc_create(fileName, NC_CLOBBER, &ncFile));
    strcpy(c, "test file for ncdataReader2");
    handle_error(nc_put_att_text(ncFile, NC_GLOBAL, "doc", strlen(c), c));
    strcpy(c, ncDataReader2Version());
    handle_error(nc_put_att_text(ncFile, NC_GLOBAL, "version", strlen(c), c));
    handle_error(nc_put_att_float(ncFile, NC_GLOBAL, "foo", NC_FLOAT, 1, &x));
    handle_error(nc_put_att_int(ncFile, NC_GLOBAL, "bar", NC_INT, 1, &y));
    return ncFile;
}


void create1DVar(int ncF) {
    char c[100];
    int ncD, ncV, ncT;
    size_t i;
    double d;
    nc_redef(ncF);
    handle_error(nc_def_dim(ncF, "time", DIM, &ncD));
    handle_error(nc_def_var(ncF, "time", NC_DOUBLE, 1, &ncD, &ncT));
    d = 2.5;
    handle_error(nc_put_att_double(ncF, ncT, NCATT_SCALE_FACTOR, NC_DOUBLE, 1, &d));
    d = 0.0;
    handle_error(nc_put_att_double(ncF, ncT, NCATT_ADD_OFFSET, NC_DOUBLE, 1, &d));
    strcpy(c, NCATT_EP_PERIODIC);
    handle_error(nc_put_att_text(ncF, ncT, NCATT_EXTRAPOLATION, strlen(c), c));
    handle_error(nc_def_var(ncF, "test1D", NC_DOUBLE, 1, &ncD, &ncV));
    d = 2.0;
    handle_error(nc_put_att_double(ncF, ncV, NCATT_SCALE_FACTOR, NC_DOUBLE, 1, &d));
    d = 3.0;
    handle_error(nc_put_att_double(ncF, ncV, NCATT_ADD_OFFSET, NC_DOUBLE, 1, &d));
    strcpy(c, NCATT_IP_AKIMA);
    handle_error(nc_put_att_text(ncF, ncV, NCATT_INTERPOLATION, strlen(c), c));
    d = 500.0; 
    handle_error(nc_put_att_double(ncF, ncV, NCATT_SMOOTHING, NC_DOUBLE, 1, &d));
    handle_error(nc_enddef(ncF));
    for (i = 0; i < DIM; i ++) {
        d = M_PI + i * 2.0 * M_PI / (DIM-1);
        handle_error(nc_put_var1_double(ncF, ncT, &i, &d));
        d = sin(d) + 0.1*RAND01();
        handle_error(nc_put_var1_double(ncF, ncV, &i, &d));
    }
}


void create2DVar(int ncF) {
    int ncD[2], ncV;
    size_t i, index[2];
    double x, y, z;
    nc_redef(ncF);
    handle_error(nc_def_dim(ncF, "npoints", NC_UNLIMITED, ncD));
    handle_error(nc_def_dim(ncF, "xyz", 3, ncD+1));
    handle_error(nc_def_var(ncF, "points", NC_DOUBLE, 2, ncD, &ncV));
    handle_error(nc_enddef(ncF));
    for (i = 0; i < NPOINTS; i ++) {
        index[0] = i;
        x = 10.0*RAND01() - 5.0;
        index[1] = 0;
        handle_error(nc_put_var1_double(ncF, ncV, index, &x));
        y = 10.0*RAND01() - 5.0;
        index[1] = 1;
        handle_error(nc_put_var1_double(ncF, ncV, index, &y));
        z = tanh(x*y);
        index[1] = 2;
        handle_error(nc_put_var1_double(ncF, ncV, index, &z));
    }

}


int main(void) {
    int ncFile;
    ncFile = createFile("testfile.nc");
    create1DVar(ncFile);
    create2DVar(ncFile);
    handle_error(nc_sync(ncFile));
    handle_error(nc_close(ncFile));
    return 0;
}
