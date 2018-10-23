#!/usr/bin/env gnuplot

set mouse
set datafile separator ","
splot 'points.csv' using 1:3:2 with points palette pointsize 2 pointtype 7
splot 'points.csv' with vectors palette pointsize 2 pointtype 7