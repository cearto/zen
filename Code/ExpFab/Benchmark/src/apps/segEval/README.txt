This program evaluates one segmentation against one ground truth segmentation.

Usage: segEval fnMesh fnSeg fnSegRef dirStat [options]
======
fnMesh : path to the mesh model
fnSeg : the segmentation to be evaluated
fnSegRef : the groundtruth segmentation 
dirStat: the directory to store the evaluation results
[-options]
  -v : output more information during running
  -CD : evaluate cut discrepancy
  -CE : evaluate consistency error
  -HD : evaluate hamming distance
  -RI : evaluate rand index
  -all : evaluate all 4 metrics above

Read function "ParseArgs" in "segEval.cpp" for more details
