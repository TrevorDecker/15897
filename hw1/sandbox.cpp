/* significantly modified by Favonia for 15210-f15 */

/* COPYRIGHT (c) 2014 Umut Acar, Arthur Chargueraud, and Michael
 * Rainey
 * All rights reserved.
 *
 * \file sandbox.cpp
 * \brief Example applications
 *
 */

#include <math.h>
#include <thread>

#include "benchmark.hpp"
#include "sort.hpp"
#include "graph.hpp"
#include "tasks.hpp"

/***********************************************************************/

void merge_sandbox() {
  // Your function from tasks.hpp is tasks::merge
  // Small instance for debugging
	sparray xs = { 0, 3, 4, 6 };
	sparray ys = { 2,4};
  sparray res = sparray(6);
  tasks::merge_par(xs, ys, res, 0, 0, 4, 0, 2);
  std::cout << "merged: " << res << std::endl;
     //Randomly generated large instance
  sparray xs_unsort = gen_random_sparray(200000);
  sparray ys_unsort = gen_random_sparray(200000);
  // Cheap (in a couple senses of the word) way of generating
  // sorted arrays without actually sorting.
  xs = scan_incl(plus_fct, 0l, xs_unsort);
  ys = scan_incl(plus_fct, 0l, ys_unsort);
  res = sparray(400000);
  tasks::merge(xs, ys, res, 0, 0, 200000, 0, 200000);
 }

void skyline_sandbox() {
  // Your function from tasks.hpp is tasks::skyl
  //Small instance for debugging
	sparray lefts = { 0, 2, 4, 6, 8, 10,30 };
	sparray heights = { 1, 2, 3, 4, 5, 6,3 };
	sparray rights = { 22, 20, 18, 16, 14, 12,32 };
  sparray xs = sparray(14);
  sparray ys = sparray(14);
  tasks::skyl(lefts, heights, rights, xs, ys, 0, 7);
  std::cout << "xs: " << xs << std::endl;
  std::cout << "ys: " << ys << std::endl;
  lefts = {4,2};
  heights = {1,2};
  rights = {18,20};
  xs = sparray(4);
  ys = sparray(4);
  tasks::skyl(lefts, heights, rights, xs, ys, 0, 2);
  std::cout << "xs: " << xs << std::endl;
  std::cout << "ys: " << ys << std::endl;


    //Randomly generated large instance
  lefts = gen_random_sparray(100000);
  rights = map([&] (value_type l) { return l + 10; }, lefts);
  heights = gen_random_sparray(100000);
  xs = sparray(200000);
  ys = sparray(200000);
  tasks::skyl(lefts, heights, rights, xs, ys, 0, 100000);
}

/*---------------------------------------------------------------------*/
/* PASL Driver */

int main(int argc, char** argv) {

  auto init = [&] {

  };
  auto run = [&] (bool) {
    // Defaultly, all of the following function calls are performed
    // on execution of this program.
    // To run just one, say, "merge", pass to this program the option:
    // -sandbox merge.
    pasl::util::cmdline::argmap_dispatch c;
    c.add("merge", [&] { merge_sandbox(); });
    c.add("skyline", [&] { skyline_sandbox(); });
    pasl::util::cmdline::dispatch_by_argmap_with_default_all(c, "sandbox");
  };
  auto output = [&] {
  };
  auto destroy = [&] {
  };
  pasl::sched::launch(argc, argv, init, run, output, destroy);
}

/***********************************************************************/
