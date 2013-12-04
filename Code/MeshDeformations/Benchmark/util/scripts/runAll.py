# Instead of running your algorithm, evaluation, analysis, and visualization
# step by step, we provide this script to automatically run them together

# You need to first configure "../parameters/algorithmList.py"

# If you added a new segmentation program, you also need to configure "runAlgorithm/config.py"
# to let the scripts know how to run your program


# import
import os, sys
root = os.getcwd()
includepath = root + '/../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *

# a function to execute programs
def myExec(cmd):
  print cmd
  os.system(cmd)


# step 1. Run a new algorithm
if doRunAlgorithm:
  os.chdir('runAlgorithm')
  myExec('python runAlgorithm.py')
  os.chdir('..')

# step 2. Evaluate
if doEvaluation:
  os.chdir('evaluate')
  myExec('python evalAlgorithm.py')
  myExec('python collectEvaluation.py')
  os.chdir('..')

# step 3. Analysis
if doAnalysis:
  os.chdir('analyze')
  myExec('python makeLinks.py')
  # the following three are for analyzing convexity and relies on Qhull, 
  # in default, they are not executed
  # myExec('python getSegmentPointSet.py')
  # myExec('python runQhull.py')
  # myExec('python extractHullInfo.py')
  myExec('python analyzeAlgorithm.py')
  myExec('python collectAnalysis.py')
  os.chdir('..') 

# step 4. Visualization
if doVisualization:
  os.chdir('visualize')
  # colored images for segmentations 
  myExec('python genImg.py')
  myExec('python genHtml.py')
  # evaluation results
  if doEvaluation:
    myExec('python plotEvaluationResults.py')
    if os.name <> 'nt':
      myExec('python createEvaluationReport.py')
  # analysis results
  if doAnalysis:
    myExec('python plotAnalysisResults.py')
    if os.name <> 'nt':
      myExec('python createAnalysisReport.py')
  os.chdir('..')

