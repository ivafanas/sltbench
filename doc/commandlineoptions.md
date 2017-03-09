# How to filter benchmarking items

Sometimes limited set of functions need to be benchmarked per run
(for example, if you are tuning one function, you do not need to measure
all functions in benchmark per run, just the one you are interested in).
It is possible to set filter by benchmark name:

This example shows how to run all functions wich name contains `MyFunction` substring:
```sh
./my_benchmark.exe --filter=.*MyFunction.*
```

Filter regular expression format corresponds to [basic case of std::regex](http://en.cppreference.com/w/cpp/regex/syntax_option_type).


# Change output format

`sltbench` output format may be changed with `--reporter` option:

```sh
./my_benchmark.exe --reporter=json
```

Supported values:
* `console` - (default) prints output as human-readable table of results
* `json` - prints output in json format
* `csv` - prints output in csv format


# Heatup

Benchmark "heatups" OS and hardware before timing. This process takes about
5 seconds (may be less). If you do not want to wait, just add a `--heatup off`
command line option to your benchmark. Be careful, disabling heatup may lead to
unstable results for the first test cases.
```bash
# Skip heatup stage, run tests ASAP.
# May lead to unstable first test cases.
./my_benchmark.exe --heatup off
```
