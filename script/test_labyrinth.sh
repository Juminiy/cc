#!/bin/bash 

make labyrinth.d

filedir="jyywiki/labs/data"
commands=(
# valid print
"./labyrinth.d -m $filedir/y_map_6_7.txt -p 0"
"./labyrinth.d --map $filedir/y_map_6_7.txt --player 0"
"./labyrinth.d --player 0 --map $filedir/y_map_6_7.txt"

# valid player move notfound,left,right,up,down
"./labyrinth.d -m $filedir/y_map_6_7_move_numnotfound.txt -p 4 --move up"
"./labyrinth.d -m $filedir/y_map_6_7_move_up.txt -p 4 --move up"
"./labyrinth.d -m $filedir/y_map_6_7_move_down.txt -p 4 --move down"
"./labyrinth.d -m $filedir/y_map_6_7_move_left.txt -p 4 --move left"
"./labyrinth.d -m $filedir/y_map_6_7_move_right.txt -p 4 --move right"

"./labyrinth.d -m $filedir/y_map_conn1.txt -p 0"
"./labyrinth.d -m $filedir/y_map_conn2.txt -p 0"
"./labyrinth.d -m $filedir/y_map_conn3.txt -p 0"

# invalid move args
"./labyrinth.d -m $filedir/y_map_6_7.txt -p 4 --move xxx"
# invalid move position
"./labyrinth.d -m $filedir/y_map_6_7.txt -p 1 --move up"
"./labyrinth.d -m $filedir/y_map_6_7.txt -p 2 --move left"
"./labyrinth.d -m $filedir/y_map_6_7.txt -p 3 --move right"
"./labyrinth.d -m $filedir/y_map_6_7.txt -p 4 --move down"

# invalid
# file not exist
"./labyrinth.d -m $filedir/y_map_not_exist.txt -p 0"
# invalid char
"./labyrinth.d -m $filedir/n_map_invalid_char.txt -p 0"
# invalid line
"./labyrinth.d -m $filedir/n_map_invalid_line.txt -p 0"
# invalid player id
"./labyrinth.d -m $filedir/y_map_6_7.txt -p 10"
# args lack map
"./labyrinth.d -p 10"
# args lack player
"./labyrinth.d -m $filedir/y_map_6_7.txt"
# land not connect
"./labyrinth.d -m $filedir/n_map_not_conn1.txt -p 0"
"./labyrinth.d -m $filedir/n_map_not_conn2.txt -p 0"
"./labyrinth.d -m $filedir/n_map_not_conn3.txt -p 0"
"./labyrinth.d -m $filedir/n_map_not_conn4.txt -p 0"
"./labyrinth.d -m $filedir/n_map_not_conn5.txt -p 0"
"./labyrinth.d -m $filedir/n_map_not_conn6.txt -p 0"
# map too huge
"./labyrinth.d -m $filedir/n_map_huge.txt -p 0"

# game version
"./labyrinth.d --version"
"./labyrinth.d -v"
# game version error
"./labyrinth.d -m $filedir/n_map_huge.txt -p 0 --version"
"./labyrinth.d -m $filedir/n_map_huge.txt -p 0 -v"
)

function collect_retcode() {
    declare -a results
    for cmd in "${commands[@]}"; do
        echo "EXEC: '$cmd'"
        eval "$cmd"
        results+=($?)
    done

    for i in "${!results[@]}"; do
        echo "'${commands[i]}' -> ${results[i]}"
    done
}

collect_retcode