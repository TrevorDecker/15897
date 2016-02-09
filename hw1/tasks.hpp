/* significantly modified by Favonia for 15210-f15 */

/* COPYRIGHT (c) 2014 Umut Acar, Arthur Chargueraud, and Michael
 * Rainey
 * All rights reserved.
 *
 * \file tasks.hpp
 * \brief File that students are to use to enter solutions to the
 *  exercises that are assigned in the book.
 *
 */

#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

#include "sparray.hpp"
#include "graph.hpp"
#include "sort.hpp"

#ifndef _TASKS_H_
#define _TASKS_H_

/***********************************************************************/

namespace tasks {

void merge_seq(const sparray& xs, const sparray& ys, sparray& res,
               long reslo, long Alo, long Ahi, long Blo, long Bhi) {

  //TODO: Fill in

}

controller_type merge_contr("merge");
void merge_par(const sparray& xs, const sparray& ys, sparray& res,
               long reslo, long Alo, long Ahi, long Blo, long Bhi){

  //TODO: Fill in

}

void merge(const sparray& xs, const sparray& ys, sparray& res,
           long reslo, long Alo, long Ahi, long Blo, long Bhi){
#ifdef SEQUENTIAL_BASELINE
  return merge_seq(xs, ys, res, reslo, Alo, Ahi, Blo, Bhi);
#else
  return merge_par(xs, ys, res, reslo, Alo, Ahi, Blo, Bhi);
#endif
}

controller_type skyline_contr("skyline");
void skyl(const sparray &lefts, const sparray &heights,
          const sparray &rights, sparray &xres, sparray &yres,
          long lo, long hi) {

  //TODO: Fill in

}


#endif /*! _TASKS_H_ */

} // end namespace

/***********************************************************************/
