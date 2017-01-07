# Metrics

Set of utilities to gather `sltbench` and competitors target metrics.
* `benchbench` - benchmark for benchmark. The target metric is the
benchmark time for the set of functions.
* `benchcompile` - benchmark for compilation time.
The target metric is compilation time of the set of performance tests.

All utilities support `sltbench` and `googlebench` backends.


# benchbench

The utility to measure benchmark time. Less time is better.

The metric is super critical. Its degradation should be fixed ASAP and is a release blocker.

Instruction:

* Install benchmark backend for timing (`sltbench` or `googlebench`)
* run `sltbench` with default parameters:
```bash
python -m benchbench.main --backend=sltbench --backend_install_path=<path_to_sltbench_installed> --pincpu 1
```
* run `googlebench` with default parameters:
```bash
python -m benchbench.main --backend=googlebench --backend_install_path=<path_to_googlebench_installed> --pincpu 1
```

Options:
* `--backend` - which backend to test (`sltbench` or `googlebench`)
* `--backend_install_path` - where backend is installed (with headers and static libs)
* `--pincpu` - cpu id to pin process (argument of `taskset -c`).
* `--toolset` - flag to switch compiler (`gcc`|`clang`)
* `--sizes` - set of sizes to test. Different problems sizes has different
behaviour in both `sltbench` and `googlebench`. Some problems may be reproduced
only for fixed subset of sizes. The default value is `all`.
* `--kinds` - set of testing function kinds. Introduced for the same reason as
`--sizes` argument. The default value is `all`.
* `--moreinfo`, `--report_format`, `--outfile`, `--runcount` - options for developer needs.


# benchcompile

The utility to measure compilation time. Less compilation time is better.

Instruction:

* Install benchmark backend for timing (`sltbench` or `googlebench`)
* run `sltbench` with default parameters:
```bash
python -m benchcompile.main --backend=sltbench --backend_install_path=<path_to_sltbench_installed>
```
* run `googlebench` with default parameters:
```bash
python -m benchcompile.main --backend=googlebench --backend_install_path=<path_to_googlebench_installed>
```

Additional options:
* `--toolset` - flag to switch compiler (`gcc`|`clang`)
* `--dataset` - set of tests to compile. `sltbench` and `googlebench` have
different functionality, so some tests cannot be compiled for `googlebench` at
all. The default dataset is the subset of tests which can be compiled both with
`sltbench` and `googlebench`.
Use `--dataset=all` to benchmark all available `sltbench` cases.
