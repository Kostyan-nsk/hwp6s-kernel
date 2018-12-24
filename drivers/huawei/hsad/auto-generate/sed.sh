#!/bin/bash

sed -i 's/codec", (unsigned int)53248,/codec", (unsigned int)65536,/' hw_EDGE_PLUS_*.c # codec
sed -i 's/fb", (unsigned int)10803,/fb", (unsigned int)10800,/' hw_EDGE_PLUS_*.c # FB
sed -i 's/gpu", (unsigned int)13312,/gpu", (unsigned int)1024,/' hw_EDGE_PLUS_*.c # GPU
sed -i 's/gralloc", (unsigned int)208432,/gralloc", (unsigned int)196608,/' hw_EDGE_PLUS_*.c # gralloc
sed -i 's/overlay", (unsigned int)58368,/overlay", (unsigned int)4,/' hw_EDGE_PLUS_*.c # overlay

sed -i 's/codec", (unsigned int)53248,/codec", (unsigned int)65536,/' hw_SOPHIA_*.c # codec
sed -i 's/fb", (unsigned int)24310,/fb", (unsigned int)24300,/' hw_SOPHIA_*.c # FB
sed -i 's/gpu", (unsigned int)13312,/gpu", (unsigned int)1024,/' hw_SOPHIA_*.c # GPU
sed -i 's/gralloc", (unsigned int)208432,/gralloc", (unsigned int)204800,/' hw_SOPHIA_*.c # gralloc
sed -i 's/overlay", (unsigned int)130048,/overlay", (unsigned int)4,/' hw_SOPHIA_*.c # overlay

