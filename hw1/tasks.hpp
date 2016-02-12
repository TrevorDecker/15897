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

	long outputLength = Ahi - Alo + Bhi - Blo;
	long xs_index = Alo;
	long ys_index = Blo;

	for(long res_index = reslo ;res_index< reslo+outputLength;res_index++){
		//handles case where all xs have been added
		if(xs_index >= Ahi){
			res[res_index] = ys[ys_index];
			ys_index++;
		}else

		//handles case where all ys have been added
		if(ys_index >= Bhi){
			res[res_index] = xs[xs_index];
			xs_index++;
		}else

		if(xs[xs_index] < ys[ys_index]){
			res[res_index] = xs[xs_index];
			xs_index++;
		}else{
			res[res_index] = ys[ys_index];
			ys_index++;
		}
	}


}


controller_type merge_contr("merge");
void merge_par(const sparray& xs, const sparray& ys, sparray& res,
               long reslo, long Alo, long Ahi, long Blo, long Bhi){
	//base case includes granularity control
	if(Ahi - Alo <= 1000 && Bhi - Blo <= 1000){
		merge_seq(xs,ys,res,reslo,Alo,Ahi,Blo,Bhi);
		return;
	}



	//pick the midpoint of xs
	long xs_mid = (Ahi + Alo)/2;
	long ys_mid;

	//find the split point of ys
	long low,high,mid;
	low = Blo;
	high = Bhi;

	//finds the greatest values less then xs_mid
	while(true){
		mid = (high + low)/2;

		if(mid == low){
			ys_mid = low;
			break;
		}

		if(ys[mid] >= xs[xs_mid]){
			high = mid-1;
		}else{
			low = mid;
		}
	}
	ys_mid = ys_mid+1;

	par::fork2([&]{
	//lower elements
			merge_par(xs,ys,res,reslo,Alo,xs_mid,Blo,ys_mid);

		},[&]{
			merge_par(xs,ys,res,
					  reslo+xs_mid+ys_mid-Alo-Blo,
					  xs_mid,
					  Ahi,
					  ys_mid,
					  Bhi);
		});
}

void merge(const sparray& xs, const sparray& ys, sparray& res,
           long reslo, long Alo, long Ahi, long Blo, long Bhi){
#ifdef SEQUENTIAL_BASELINE
  return merge_seq(xs, ys, res, reslo, Alo, Ahi, Blo, Bhi);
#else
  return merge_par(xs, ys, res, reslo, Alo, Ahi, Blo, Bhi);
#endif
}

void merge_sky_seq(const sparray &xin_left, const sparray &yin_left,
				   const sparray &xin_right, const sparray &yin_right,
				   sparray &xres,
				   sparray &leftHeights_res, sparray &rightHeights_res,
				   long res_lo,
				   long left_lo,long left_hi,
				   long right_lo,long right_hi){

	long left_len = left_hi - left_lo;
	long right_len = right_hi - right_lo;
	long left_index = 0;
	long right_index = 0;

	for(long res_index = 0 ;res_index< left_len+right_len;res_index++){
		//handles case where all xs have been added
		if(left_index >= left_len){
			xres[res_lo+res_index] = xin_right[right_lo+right_index];
			leftHeights_res[res_lo+res_index] = -1;
			rightHeights_res[res_lo+res_index] = yin_right[right_lo+right_index];
			right_index++;
		}else

		//handles case where all ys have been added
		if(right_index >= right_len){
			xres[res_lo+res_index] = xin_left[left_lo+left_index];
			leftHeights_res[res_lo+res_index] = yin_left[left_lo+left_index];
			rightHeights_res[res_lo+res_index] = -1;
			left_index++;
		}else

		if(xin_left[left_lo+left_index] >= xin_right[right_lo+right_index]){
			xres[res_lo+res_index] = xin_right[right_lo+right_index];
			leftHeights_res[res_lo+res_index] = -1;
			rightHeights_res[res_lo+res_index] = yin_right[right_lo+right_index];
			right_index++;
		}else{
			xres[res_lo+res_index] = xin_left[left_lo+left_index];
			leftHeights_res[res_lo+res_index] = yin_left[left_lo+left_index];
			rightHeights_res[res_lo+res_index] = -1;
			left_index++;
		}
	}

}




void merge_sky_par(const sparray &xin_left, const sparray &yin_left,
				   const sparray &xin_right, const sparray &yin_right,
				   sparray &xres,
				   sparray &leftHeights_res, sparray &rightHeights_res,
				   long lo_res,
				   long left_lo,long left_hi,
				   long right_lo,long right_hi){

	//base case includes granularity control
	if(left_hi - left_lo <= 1000 && right_hi - right_lo <= 1000){
		merge_sky_seq(xin_left, yin_left,
					  xin_right, yin_right,
					  xres,
					  leftHeights_res,rightHeights_res,
					  lo_res,
					  left_lo,left_hi,
			          right_lo,right_hi);
		return;
	}


	//pick the midpoint of xs
	long left_mid = (left_hi+ left_lo)/2;
	long right_mid;

	//find the split point of ys
	long low,high,mid;
	low = right_lo;
	high = right_hi;

	//finds the greatest values less then xs_mid
	while(true){
		mid = (high + low)/2;


		if(low == mid){
			if(xin_right[mid] >= xin_left[left_mid]){
				right_mid = mid -1;
			}else{
				right_mid = low;
			}
			break;
		}

		if(xin_right[mid] >= xin_left[left_mid]){
			high = mid-1;
		}else{
			low = mid;
		}
	}
	right_mid = right_mid+1;

	par::fork2([&]{
	//lower elements
			merge_sky_par(xin_left, yin_left,
						  xin_right, yin_right,
						  xres,
						  leftHeights_res,rightHeights_res,
						  lo_res,left_lo,left_mid,right_lo,right_mid);
		},[&]{
			merge_sky_par(xin_left, yin_left,
						  xin_right, yin_right,
						  xres,
						  leftHeights_res,rightHeights_res,
						  lo_res+left_mid+right_mid-left_lo-right_lo,
						  left_mid,left_hi,right_mid,right_hi);
		});

}



controller_type skyline_contr("skyline");
void skyl(const sparray &lefts, const sparray &heights,
          const sparray &rights, sparray &xres, sparray &yres,
          long lo, long hi) {

	//base case
	if(hi == lo + 1){
		//top left corrner
		xres[0] = lefts[lo];
		yres[0] = heights[lo];
		//bottom right
		xres[1] = rights[lo];
		yres[1] = 0;
		return;
	}

	//base case
	if(hi == lo){
		return;
	}


	long mid = (hi + lo)/2;
	sparray xinLeft = sparray(2*(mid - lo));
	sparray yinLeft = sparray(2*(mid - lo));
	sparray xinRight = sparray(2*(hi - mid));
	sparray yinRight = sparray(2*(hi - mid));



	//otherwise we will need to split the input
	par::fork2([&]{skyl(lefts,heights,rights,xinLeft,yinLeft,lo,mid);},[&]{
			skyl(lefts,heights,rights,xinRight,yinRight,mid,hi);});

	//now we need to combine the results
	sparray leftHeights_res  = sparray(2*(hi-lo));
	sparray rightHeights_res = sparray(2*(hi-lo));

	//merge
	merge_sky_par(xinLeft,yinLeft,
				  xinRight,yinRight,
				  xres,leftHeights_res,rightHeights_res,0,0,xinLeft.size(),0,xinRight.size());

	//copy scan
	par::fork2([&]{
	leftHeights_res = scan_incl([&](long a,long b){if(b == -1){return a;}else{return b;}},(long)-1,leftHeights_res);
},[&]{
			rightHeights_res = scan_incl([&](long a,long b){if(b == -1){return a;}else{return b;}},(long)-1,rightHeights_res);
});


	//take max
	yres = tabulate([&] (long i) {if(leftHeights_res[i] > rightHeights_res[i]){ return leftHeights_res[i];} else{return rightHeights_res[i];}},leftHeights_res.size());

	return;
}


#endif /*! _TASKS_H_ */

} // end namespace

/***********************************************************************/
