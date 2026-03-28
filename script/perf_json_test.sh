#!/bin/bash

bin="json_test.d"
fgdir="data/git/FlameGraph"
pnbase="data/perf/codejson"

make $bin

perf record -o $pnbase.data -g ./$bin data/test/code.json

# report
# perf report -i $pnbase.data

# mem
perf script -i $pnbase.data  > $pnbase.perf
./"$fgdir"/stackcollapse-perf.pl $pnbase.perf > $pnbase.folded
./"$fgdir"/flamegraph.pl --title "json decode perf(mem)" $pnbase.folded > "$pnbase"_mem.svg