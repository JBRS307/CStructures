#!/bin/bash

rm -rf sanitize/ && cmake -H. -Bsanitize -DCMAKE_BUILD_TYPE=SANITIZE