#ifndef __POW_DD_H
#define __POW_DD_H

#include "f2c.h"

#ifdef KR_headers
double pow();
double pow_dd(ap, bp) doublereal *ap, *bp;
#else
#include <math.h>
double pow_dd(doublereal *ap, doublereal *bp)
#endif
{
return(pow(*ap, *bp) );
}

#endif /*__POW_DD_H */
