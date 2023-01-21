# ScatterGather

The target for this implementation is to give a very simplified demonstration of quivelent implementation for Scatter Gather operations which are native to DMA Engines as hardware block.
More information @ https://en.wikipedia.org/wiki/Gather/scatter_(vector_addressing)

Implmentaiton Holds below blocks:

InputBlocks: 
These are the blocks which represents various buffers submitted by user-space OS for DMA able buffer. The recognition to start the process is most likely responsiblitiy for driver layer- which is note covered as part of this.

OutputBlock:
Since DMA engines are optimized for large contiguous memory copy operations. To Utilize the underlaying hardware at best, the **Scattered** Buffers are **Gathered** into larger chunks.


Implementation:
The implementation is basic calloc based which is used to allocate smaller and larger buffer equivelents of the DMA user and driver space buffers. There are respective test functions for the same.
