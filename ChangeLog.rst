
2.0.0
------------------

Key changes
~~~~~~~~~~~

* Speeded up benchmarking of fast functions (less than 0.00001 sec) in about
  3x - 100x times.

* Removed 20-nanoseconds (approx.) overhead on function call, which is critical
  for functions with execution time less then 0.0000005 sec (500 nanoseconds).
  For other functions 20 nanoseconds fits  into timing error and does not influe 
  on performance results.
 
* Significantly improved reproducibility, quality and benchmarking time for
  functions with execution time about tens of nanoseconds.
 
* Now sltbench is about 4x times faster than googlebench on training dataset and
  produce more stable results (see metrics section below).
 
* Added tool to compare performance results. Usable to check was functions
  dataset speeded up or not. See ``tools/compare/main.py``
  
* Added tools to compare key project metrics for sltbench and competitors like
  googlebench (see ``tools/metrics``): benchmark time, performance results
  reproducibilty (rel. error) and compilation time.

* Added ``IMPRECISE`` status for datasets when sltbench cannot guarantee
  reproducible results. If you see ``IMPRECISE`` status, the problem is in a
  test in 99% cases. See the corresponding documentation notes about the most
  frequent cases of imprecise behaviour.

Other
~~~~~

* Added ``--heatup off`` command line option to skip heatup stage.

* Added ``--reporter`` command line option to choose report format.

* Added warning log about the same test suite registered twice.


1.1.0
------------------

Key changes
~~~~~~~~~~~

* Added lazy generators to save RAM for large input datasets.

* Speeded up benchmarking of fast functions (less than 1 ms) in about 20%.

Other
~~~~~

* Added tarvis and appveyor CI.

* Added build instructions to README.


1.0.1
------------------

* Fixed ``--filter`` option when positional command line argument is provided.


1.0.0
------------------

* Initial release.

* Added benchmarks for procedures, fixtures, arguments and generators.

* Added bencmharks filtering by command line ``--filter`` option with regexp.

* Added instruction HOWTO implement and run benchmarks.

* Measure algorithm is learned on training dataset.

* The first sltbench release test functions about 3x times faster than 
  googlebench. Looks reasonable.
