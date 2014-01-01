
HOW TO RUN ANALYSIS EXPERIMENTS
===============================

You may follow the steps below to run segmentation analysis experiments

1. Run "python makeLinks.py" to link all segmentations generated with
numSegment set as the median number of human segmentations for that model

2. Skip if you do not want to analyze convexity and compactness (which requires
QHull {www.qhull.org} to be installed as well as additional processing time). 
Otherwise, uncomment the line "cmd += '-convexity %s' % pathHull"
in "analyzeAlgorithm.py", and proceed as suggested below:  
   run "python getSegmentPointSet.py"  
   run "python runQhull.py" 
   run "python extractHullInfo.py"
Please be aware that the three python scripts above are only tested in Linux

3. Run "python analyzeAlgorithm.py"
About 0.5 hour to analyze each algorithm, and several hours to analyze the
benchmark. You may select or delete items to analyze in function
analyze(..)  

4. Run "python collectAnalysis.py"
The analysis data will be in "../../../analysis/raw-analysis/$(AlgorithmName)/stat-processed/"

We also provide scripts to generate plots of statistics of properties (using Matlab), 
see "../visualize/README.txt" for details. 
 
