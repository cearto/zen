This program outputs all vertices of each segment in a given segmentation, 
which could be used as inputs to QHull.

Usage: outputSegments fnMesh [-seg fnSeg] dirOutput [-v]
======
fnMesh : path to the mesh model
-seg fnSeg : path to the segmentation for fnMesh
dirOutput : directory to store vertices for each segment(".segment" file)
-v : output more information during running

