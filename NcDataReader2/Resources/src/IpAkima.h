/*  ncDataReader2 - interpolating reading of 1D data sets
    Copyright (C) 2004  Joerg Raedler <joerg@dezentral.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef IPAKIMA_H
#define IPAKIMA_H

#include "ncDataReader2.h"
#include "config.h"
#include "Chain.h"

double ncVar1DGetAkima(NcVar1D *var, double x);

/* internal struct */
struct _AkimaData {
    size_t dim;
    double *X;
    double *Y;
    double *B;
    double *C;
    double *D;
};
typedef struct _AkimaData AkimaData;

/* cached handling of parameters */
Item *akimaCacheNew(int);
void  akimaCacheFree(Item *);
void  akimaCacheReset(Item *);

#endif /* IPAKIMA_H */
