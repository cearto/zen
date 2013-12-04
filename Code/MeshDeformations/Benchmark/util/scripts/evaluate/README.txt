
HOW TO RUN EVALUATION
=====================

Before running evaluation experiments, please make sure
- you have added your algorithm NEWNAME in the algorithmList
  "../../parameters/algorithmList.py"
- you properly pre-processed your segmentation results
- and your processed segmentations are in "../../../data/seg/$(NEWNAME)" 

Then you could run the experiments in the following sequence:
1. run "python evalAlgorithm.py" to get the evaluation for algorithms
About 1 hour to evaluate each algorithm  
2. run "python collectEvaluation.py" 

Final evaluation results will be in "../../../evaluation/eval-processed/stat/". 

Files for evaluation results are named as "ByX.Y", where "X" describes the method of selecting the
number of segments for those algorithms that do not do this automatically, and
"Y" describes the evaluation metric. There are 21 rows in the file, of which the first 
row is the mean of the whole data set, and the next 20 rows are the mean for
each category (the 15th line for category 14 is not included in our statistics)  

In addition, for each "ByX.Y", there is a "ByX.Y.xls", which is an easy-to-read
spreadsheet for the same data in ByX.Y (but not showing numbers for
category 14)

If Matlab is installed, you can use our scripts to plot the evaluation results, see 
"../visualize/README.txt" for details
  
