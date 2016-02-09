/* COPYRIGHT (c) 2014 Umut Acar, Arthur Chargueraud, and Michael
 * Rainey
 * All rights reserved.
 *
 * \file sort.hpp
 * \brief Sorting algorithms
 *
 */

#include <cstring>
#include <cmath>

#include "native.hpp"
#include "sparray.hpp"

#ifndef _MINICOURSE_SORT_H_
#define _MINICOURSE_SORT_H_

/***********************************************************************/

long nlogn(long n) {
  return pasl::data::estimator::annotation::nlgn(n);
}

/*---------------------------------------------------------------------*/
/* Sequential sort */

void in_place_sort(sparray& xs, long lo, long hi) {
  long n = hi-lo;
  if (n < 2)
    return;
  std::sort(&xs[lo], &xs[hi-1]+1);
}

void in_place_sort(sparray& xs) {
  in_place_sort(xs, 0l, xs.size());
}

sparray seqsort(const sparray& xs) {
  sparray tmp = copy(xs);
  in_place_sort(tmp);
  return tmp;
}

sparray seqsort(const sparray& xs, long lo, long hi) {
  sparray tmp = slice(xs, lo, hi);
  in_place_sort(tmp);
  return tmp;
}

/***********************************************************************/

#endif /*! _MINICOURSE_SORT_H_ */
