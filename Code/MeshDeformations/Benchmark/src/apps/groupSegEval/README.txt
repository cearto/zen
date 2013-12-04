This program takes in all segmentations for one model to evaluate in a "leave
one out" scheme. (We used it to evaluate human generated segmentations)

Usage: groupSegEval fnMesh dirIn dirOut modelId nSegs [options]
======
fnMesh : path to the mesh model
dirIn : the directory containing the segmentation files
dirOut : the directory to store evaluation results
modelId : id of the mesh model
nSegs: number of segmentations in dirIn
[options]
  -v : output more information during running  
  -CD : evaluate cut discrepancy
  -CE : evaluate consistency error
  -HD : evaluate hamming distance
  -RI : evaluate rand index
  -all : evaluate all 4 metrics above      

Read function "ParseArgs" in "groupSegEval.cpp" for more details
