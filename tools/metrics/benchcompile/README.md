# benchcompile

Utility to measure compilation time of performance tests.
`sltbench` and `googlebench` backends are supported.


# Instruction

* Install benchmark backend for timing (`sltbench` or `googlebench`)
* run `sltbench` with default parameters:
```bash
python -m benchcompile --backend=sltbench --backend_install_path=<path_to_sltbench_installed>
```
* run `googlebench` with default parameters:
```bash
python -m benchcompile --backend=googlebench --backend_install_path=<path_to_googlebench_installed>
```

Additional options:
* `--toolset` - flag to switch compiler (`gcc`|`clang`)
* `--dataset` - set of tests to compile. `sltbench` and `googlebench` have
different functionality, so some tests cannot be compiled for `googlebench` at all.
The default dataset is the subset of tests which can be compiled both with
`sltbench` and `googlebench`. Use `--dataset=all` to benchmark all available `sltbench` cases.
