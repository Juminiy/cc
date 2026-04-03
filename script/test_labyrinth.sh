#!/bin/bash 

make labyrinth.d

# valid print
./labyrinth.d -m y_map_6_7.txt -p 0
./labyrinth.d --map y_map_6_7.txt --player 0
./labyrinth.d --player 0 --map y_map_6_7.txt

# valid add player
./labyrinth.d -m y_map_6_7.txt -p 4 --move up
# valid move player
./labyrinth.d -m y_map_6_7.txt -p 4 --move down
# invalid move args
./labyrinth.d -m y_map_6_7.txt -p 4 --move xxx
# invalid move position
./labyrinth.d -m y_map_6_7.txt -p 4 --move up

# invalid
# file not exist
./labyrinth.d -m y_map_not_exist.txt -p 0
# invalid char
./labyrinth.d -m n_map_invalid_char.txt -p 0
# invalid line
./labyrinth.d -m n_map_invalid_line.txt -p 0
# invalid player id
./labyrinth.d -m y_map_6_7.txt -p 10
# args lack map
./labyrinth.d -p 10
# args lack player
./labyrinth.d -m y_map_6_7.txt
# land not connect
./labyrinth.d -m n_map_not_conn1.txt -p 0
./labyrinth.d -m n_map_not_conn2.txt -p 0
# map too huge
./labyrinth.d -m n_map_huge.txt -p 0

# game version
./labyrinth.d --version
./labyrinth.d -v
# game version error
./labyrinth.d -m n_map_huge.txt -p 0 --version
./labyrinth.d -m n_map_huge.txt -p 0 -v