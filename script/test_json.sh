#!/bin/bash

for fpy in json_test_checker.py  json_test_encoding.py  json_test_parsing.py  json_test_roundtrip.py  json_test_transform.py json_test_yyjson.py
do
    python3 python/$fpy
done