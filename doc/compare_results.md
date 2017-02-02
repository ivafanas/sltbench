# How to compare performance results

`sltbench` project contains tool to compare performance results.
 Utility is located at `tools` directory. It accepts benchmarks output **in json
 format**. `sltbench` and `googlebench` backends are supported.

 Usage:
```bash
cd tools
python -m compare.main --backend=sltbench --file_prev run1.json --file_next run2.json
```

Options:
* `--backend` - backend which produced output. Supported values: `sltbench` and
`googlebench`. Default value: `sltbench`.
* `--file_prev` - file with performance results in json format.
* `--file_next` - file with performance results in json format.
* `--format` - output format. Supported values: `json|readable`. Default value:
`readable`.

Utility proposes that `file_next` is compared versus `file_prev`, i.e. ratio greater than
1 means speedup and ratio less than 1 means slowdown.

`file_prev` and `file_next` must contains the same dataset of measured functions
with arguments, otherwise it is impossible to compare different datasets. Utility
will raise an exception if datasets are not equal.

Output example for `readable` format:
```
function                              prev_ns        next_ns   ratio
InsertToSetSorted/1000                  95517          95662   0.998
InsertToSetSorted/10000               1348704        1349769   0.999
InsertToSetSorted/100000             19488180       19485450   1.000
InsertToSetSorted/1000000           272967626      272922463   1.000
Sort_Fixture                           862500         861708   1.001
```
