# libjson

## testing
1. yyjson/test data to test correctness
### json behavior
1. encode object: duplicate name ERROR
2. decode object: duplicate name ERROR
### parseing
1. (✅)how to recognize '\t' with ' ', '\n' with '', '\"' with '"', ...
2. how to recognize character:
(✅)![null_del](pic/char_null_del.png),
(❌)![formfeed](pic/char_formfeed.png),
(✅)![\u2060](pic/char_u2060.png),
1. (✅) object or array exceeded maxdepth ![stackoverflow](pic/invalid_json_stack_overflow.png)
2. (❌)at end of file char \000

## performance
### DECODE WEAKNESS
1. mem: usage huge
    > Golang Std code.json
    performance is weak:
    ```
    # ts
    bssize = 7238262B
    json decode time: 9262.6630ms
    json encode time: 13.3800ms

    # mem
    total heap usage: 689,401 allocs, 689,401 frees, 6, 294,328,569 bytes 
    ```
2. ts: decode time 300' slower than gostd 
### TODO
1. json_object use array, not rb_tree
2. use pperf to opt core function
3. analyse the profiling
   