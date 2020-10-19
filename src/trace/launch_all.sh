#!/bin/bash

for i in /scratch/figual/videosJVT/*.yuv; do echo $i; ./launch.sh $i options ; done

