
MAIN CONFIGURATION
==================
algorithmList.py
  The list of algorithms to be included in the evaluation and analysis, you
may need to edit the list if you add new algorithms for study. 


PARAMETERS FOR NUMBER OF SEGMENTS
=================================
nSeg-BySegmentation.txt
  Observed number of segments for each manual generated segmentation. There
are 400 lines, the j th element(starting from 0) on line i(starting from 1) is
the number of segments for "i_j.seg" in the human benchmark. Ignore line 261
to 280 which corresponds to category spring.  

nSeg-ByModel.txt
  For all human segmentations of each mesh model, we calculate the mode number
of segments (M). If there are multiple M, we take the one closest to the median number 
of segments. Here, the number in line i(starting from 1) is the M for model i.  

nSeg-ByCategory.txt
  The number in line i corresponds to the mode number of segments for the
object category that model i belongs to.  

nSeg-ByDataset.txt
  The 400 lines are 7, which is the average of all numbers in nSeg-ByCategory.
We regard it as the average number of segments people will choose for the mesh
model data set.

nSegDiff.txt
  Line i has the different numbers of segments appearing in the above four
file for model i. It is used when a new algorithm need to know the number of
segments in advance. 


PARAMETERS FOR OTHERS
=====================

camera.txt
  Camera parameters for generating snapshots of a mesh. There are 400 lines,
line i is the parameters for model i.

Makefile.evalReport
  The makefile for creating the evaluation and analysis report

style.css
  The style file to be used for webpages

