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
  sparray xs = { 0, 6 };
  sparray ys = { 2, 4 };
  sparray res = sparray(4);
  tasks::merge(xs, ys, res, 0, 0, 2, 0, 2);
  std::cout << "merged: " << res << std::endl;
  /*
    //Randomly generated large instance
  sparray xs_unsort = gen_random_sparray(100000000);
  sparray ys_unsort = gen_random_sparray(100000000);
  // Cheap (in a couple senses of the word) way of generating
  // sorted arrays without actually sorting.
  sparray xs = scan_incl(plus_fct, 0l, xs_unsort);
  sparray ys = scan_incl(plus_fct, 0l, ys_unsort);
  sparray res = sparray(200000000);
  tasks::merge(xs, ys, res, 0, 0, 100000000, 0, 100000000);
  */
}

void skyline_sandbox() {
  // Your function from tasks.hpp is tasks::skyl
  //Small instance for debugging
  sparray lefts = { 0, 2, 4, 6, 8, 10 };
  sparray heights = { 1, 2, 3, 4, 5, 6 };
  sparray rights = { 22, 20, 18, 16, 14, 12 };
  sparray xs = sparray(12);
  sparray ys = sparray(12);
  tasks::skyl(lefts, heights, rights, xs, ys, 0, 6);
  std::cout << "xs: " << xs << std::endl;
  std::cout << "ys: " << ys << std::endl;
  /*
    //Randomly generated large instance
  sparray lefts = gen_random_sparray(10000000);
  sparray rights = map([&] (value_type l) { return l + 10; }, lefts);
  sparray heights = gen_random_sparray(10000000);
  sparray xs = sparray(20000000);
  sparray ys = sparray(20000000);
  tasks::skyl(lefts, heights, rights, xs, ys, 0, 10000000);
  */

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

