#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/src/openssl-1.0.2f
./build/UnitTest --gtest_filter=TftpTaskLv01.* $@
#./build/UnitTest $@

