# sltbench

[![Build Status](https://travis-ci.org/ivafanas/sltbench.svg)](https://travis-ci.org/ivafanas/sltbench)
[![Build status](https://ci.appveyor.com/api/projects/status/s0q3gnqprrqsahda?svg=true)](https://ci.appveyor.com/project/ivafanas/sltbench)

C++ benchmark tool. Practical, stable and fast performance testing framework.
- practical - came up from daily production tasks. Usability matters
- stable - correctness and reproducibility is a goal
- fast - if results are stable, they should be produced as soon as possible

This tool is trained to gain about 5% measure stability (depends on function,
input and context). Of course, it cannot strongly guarantee 5% stability (some
functions are unstable itself, nothing helps), but it tends to.


# Why sltbench

The closest and the most famous analogue is googlebench - microbenchmarking
library from Google engineers.

Key features of `sltbench` are:
* sltbench is about *4.7x times faster* than googlebench (test execution time).
* sltbench produces results with less timing error, which matters for
performance results reproducibility. About 5x times improvement for testing
dataset.
* Supports correct mutable fixtures.
* Supports lazy arguments generation including reading from file,
  autogeneration etc.. (critical for large input datasets which does not fit
  into RAM all together)
* Output customization, separate tools for performance results comparison and
  more.

### measure time

(Results below are obtained on 4 cores i7-2600 CPU @ 3.40GHz, 8Gb RAM
for sltbench dataset; see "Metrics" section for details; see
[How to benchmark](doc/howtobenchmark.md) section for instructions)

|                      |sltbench|googlebench|nonius|
|----------------------|:------:|:---------:|:----:|
| benchmark time, sec  | 87.45  |   416.44  |482.03|
| average rel error, % |  0.4%  |    5.6%   |  2.6%|
| maximum rel error, % |  2.9%  |   15.1%   | 60%  |

4.7x times speedup might be useful for projects with big performance tests count. For the original project regression performance testing tooks about a week and testing time reduction matters.

Less relative error means:
* Less false speedups/slowdowns you get during testing. Economy of developer
  time.
* More precise performance changes may be obtained. If testing tool provides
  15% relative error than 10% speedup couldn't be measured.

### compilation time

Sltbench contains improvements on compilation time of benchmarks (not library!)
for the most popular api since 2.4.0. Benchmarks for functions without arguments
and driver launch might be implemented in a more lightweight manner. See
[Advanced user guide](doc/advanced_user_guide.md) for explanation how to reuse
improvements.

(benchcompile metric results below are obtained on Intel(R) Core(TM) i5-8400 CPU
@ 2.80GHz, clang-6.0.0, dataset=simple for optimized cases only)

|                    | compilation time, sec |
|--------------------|:---------------------:|
| sltbench 2.4.0     | 2.5                   |
| googlebench v1.5.1 | 16.5                  |


# Motivating example

The following code registers `my_function` for timing. Just compile it (do not
forget about includes) and the executable file will test `my_function` for
performance.

```c++
void my_function()
{
	std::vector<size_t> v(100000, 0);
	std::sort(v.begin(), v.end());
}

SLTBENCH_FUNCTION(my_function);

SLTBENCH_MAIN();
```


# Documentation

* [Build instructions](doc/build.md)
* [Examples](doc/examples.md)
* [How to benchmark](doc/howtobenchmark.md) - follow this
  instruction before you run any performance test
* [How to analyse performance results](doc/howto_analyze_results.md)
* [How to compare performance results](doc/compare_results.md)
* [Advanced command line options](doc/commandlineoptions.md)
* [Advanced user guide](doc/advanced_user_guide.md)
* [Critical metrics for sltbench project and evaluation tools](tools/metrics/README.md)


# License

sltbench is released under the Apache 2.0 license
