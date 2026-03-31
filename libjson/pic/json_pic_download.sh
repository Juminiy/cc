#!/bin/bash

for png in "object" "array" "value" "string" "number" "whitespace";
do
    wget https://www.json.org/img/$png.png -O libjson/pic/$png.png 
done