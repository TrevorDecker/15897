/* significantly modified by Favonia for 15210-f15 */

/* COPYRIGHT (c) 2014 Umut Acar, Arthur Chargueraud, and Michael
 * Rainey
 * All rights reserved.
 *
 * \file bench.cpp
 * \brief Benchmarking driver
 *
 */

#include "benchmark.hpp"
#include "sort.hpp"
#include "graph.hpp"
#include "tasks.hpp"

/***********************************************************************/

loop_controller_type almost_sorted_sparray_contr("almost_sorted_sparray");

// returns an array that is sorted up to a given number of swaps
sparray almost_sorted_sparray(long s, long n, long nb_swaps) {
  sparray tmp = sparray(n);
  par::parallel_for(almost_sorted_sparray_contr, 0l, n, [&] (long i) {
    tmp[i] = (value_type)i;
  });
  for (long i = 0; i < nb_swaps; i++)
    std::swap(tmp[random_index(2*i, n)], tmp[random_index(2*i+1, n)]);
  return tmp;
}

loop_controller_type exp_dist_sparray_contr("exp_dist_sparray");

// returns an array with exponential distribution of size n using seed s
sparray exp_dist_sparray(long s, long n) {
  sparray tmp = sparray(n);
  int lg = log2_up(n)+1;
  par::parallel_for(exp_dist_sparray_contr, 0l, n, [&] (long i) {
    long range = (1 << (random_index(2*(i+s), lg)));
    tmp[i] = (value_type)hash64shift((long)(range+random_index(2*(i+s), range)));
  });
  return tmp;
}

/*---------------------------------------------------------------------*/
/* Benchmark framework */

using thunk_type = std::function<void ()>;

using benchmark_type =
  std::pair<std::pair<thunk_type,thunk_type>,
            std::pair<thunk_type, thunk_type>>;

benchmark_type make_benchmark(thunk_type init, thunk_type bench,
                              thunk_type output, thunk_type destroy) {
  return std::make_pair(std::make_pair(init, bench),
                        std::make_pair(output, destroy));
}

void bench_init(const benchmark_type& b) {
  b.first.first();
}

void bench_run(const benchmark_type& b) {
  b.first.second();
}

void bench_output(const benchmark_type& b) {
  b.second.first();
}

void bench_destroy(const benchmark_type& b) {
  b.second.second();
}

/*---------------------------------------------------------------------*/
/* Benchmark definitions */

benchmark_type reduce_bench() {
  long n = pasl::util::cmdline::parse_or_default_long("n", 1l<<20);
  sparray* inp = new sparray(0);
  value_type* result = new value_type;
  auto init = [=] {
    *inp = fill(n, 1);
  };
  auto bench = [=] {
    *result = tasks::reduce(plus_fct, 0l, *inp);
  };
  auto output = [=] {
    std::cout << "result " << *result << std::endl;
  };
  auto destroy = [=] {
    delete inp;
    delete result;
  };
  return make_benchmark(init, bench, output, destroy);
}

benchmark_type mergesort_bench() {
  long n = pasl::util::cmdline::parse_or_default_long("n", 1l<<20);
  sparray* inp = new sparray(0);
  sparray* outp = new sparray(0);
  auto sort_fct = [] (sparray& xs) { return tasks::mergesort(xs); };
  auto init = [=] {
    pasl::util::cmdline::argmap_dispatch c;
    c.add("random", [=] {
      *inp = gen_random_sparray(n);
    });
    c.add("almost_sorted", [=] {
      long nb_swaps = pasl::util::cmdline::parse_or_default_long("nb_swaps", 1000);
      *inp = almost_sorted_sparray(1232, n, nb_swaps);
    });
    c.add("exponential_dist", [=] {
      *inp = exp_dist_sparray(12323, n);
    });
    c.find_by_arg_or_default_key("generator", "random")();
  };
  auto bench = [=] {
    *outp = sort_fct(*inp);
  };
  auto output = [=] {
    std::cout << "result " << (*outp)[outp->size()-1] << std::endl;
  };
  auto destroy = [=] {
    delete inp;
    delete outp;
  };
  return make_benchmark(init, bench, output, destroy);
}

benchmark_type bfs_bench() {
  adjlist* graphp = new adjlist;
  sparray* visitedp = new sparray;
  std::string fname = pasl::util::cmdline::parse_or_default_string("fname", "");
  vtxid_type source = pasl::util::cmdline::parse_or_default_long("source", (value_type)0);
  auto bfs_fct = [&] (const adjlist& graph, vtxid_type source) { return tasks::bfs(graph, source); };
  if (fname == "")
    pasl::util::atomic::fatal([] { std::cerr << "missing filename for graph: -fname filename"; });
  auto init = [=] {
    graphp->load_from_file(fname);
  };
  auto bench = [=] {
    //*visitedp = bfs(*graphp, source);
    *visitedp = bfs_fct(*graphp, source);
  };
  auto output = [=] {
    long nb_visited = sum(*visitedp);
    std::cout << "nb_visited\t" << nb_visited << std::endl;
  };
  auto destroy = [=] {
    delete graphp;
    delete visitedp;
  };
  return make_benchmark(init, bench, output, destroy);
}

/*---------------------------------------------------------------------*/
/* PASL Driver */

int main(int argc, char** argv) {

  benchmark_type bench;

  auto init = [&] {
    pasl::util::cmdline::argmap<std::function<benchmark_type()>> m;
    m.add("reduce",               [&] { return reduce_bench(); });
    m.add("mergesort",            [&] { return mergesort_bench(); });
    m.add("bfs",                  [&] { return bfs_bench(); });

    bench = m.find_by_arg("bench")();
    bench_init(bench);
  };
  auto run = [&] (bool) {
    bench_run(bench);
  };
  auto output = [&] {
    bench_output(bench);
  };
  auto destroy = [&] {
    bench_destroy(bench);
  };
  pasl::sched::launch(argc, argv, init, run, output, destroy);
}

/***********************************************************************/

