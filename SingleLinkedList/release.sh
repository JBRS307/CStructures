#!/bin/bash

rm -rf release/ && cmake -H. -Brelease -DCMAKE_BUILD_TYPE=Release