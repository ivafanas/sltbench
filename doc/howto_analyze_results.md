# How to analyse results

Sltbench prints the following results for each test case:
* `benchmark` - name of the test
* `arg` - string representation of the test case argument (if any)
* `status` - measure status
* `time(ns)` - measure result in nanoseconds

Output example for `console` reporter:
```
benchmark                      arg                      status               time(ns)
InsertToSetSorted              1000                     ok                    2052683
InsertToSetSorted              10000                    ok                   36229724
InsertToSetSorted              100000                   ok                  361036546
Shuffle_Fixture                1/1                      IMPRECISE                1463
Shuffle_Fixture                2/1                      IMPRECISE                1463
SimpleFunction_SHOULD_THROW                             CRASHED                     0
SimpleShortFunction                                     ok                        776
```
Other reporter options documented [here] (commandlineoptions.md).


## Statuses

`status` is one of the `{ok, CRASHED, IMPRECISE}`
* `ok` - benchmark has been run and sltbench did all he can to achieve
correct and reproducible result.
* `CRASHED` - an exception has been thrown during testing. `time(ns)` for this
test case will be set to zero.
* `IMPRECISE` - benchmark has been run, but it is hard to guarantee
correct and reproducible performance results for test case. How to deal with
imprecise status see in [advanced user guide] (advanced user guide.md).
