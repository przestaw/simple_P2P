#!/bin/bash
work_dir=${PWD}

rm -R ${work_dir}/build

mkdir ${work_dir}/build
mkdir ${work_dir}/build/Relase
mkdir ${work_dir}/build/Debug

{
cd ${work_dir}/build/Relase;
cmake ../../ -DCMAKE_BUILD_TYPE="RELASE"
}

{
cd ${work_dir}/build/Debug;
cmake ../../ -DCMAKE_BUILD_TYPE="DEBUG"
}

