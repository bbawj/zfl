# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zflclient)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

set(my_dir /home/bawj/projects/fyp/zephyrproject/zephyr/zfl/zflserver)
# set(gen_dir ${ZEPHYR_BINARY_DIR}/include/generated/)
# set(source_file src/data/train-images-1)
# generate_inc_file_for_target(app ${source_file} ${gen_dir}/train-images.bin.inc)
# set(source_file src/data/train-labels-1)
# generate_inc_file_for_target(app ${source_file} ${gen_dir}/train-labels.bin.inc)

include(${ZEPHYR_BASE}/samples/net/common/common.cmake)
