# this script uses Qhull, 
# please install it by yourself   
# see www.qhull.org

import os, sys

path = '../../../analysis/raw-analysis/' # assume the path has existed from the previous step
tool = 'qhull' # should be the place where you install QHull

# include algorithm list
includepath = os.getcwd() + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *
algorithms = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].needAnalysis == False):
    continue 
  algorithms.append(allAlgorithms[i].FullName)

for a in algorithms:
  print 'Run Qhull for %s' % a
  path1 = path + a + '/'
  pathIn = path1 + 'segments/'
  pathOut = path1 + 'qhull/'
  if not os.path.exists(pathOut):
    os.mkdir(pathOut)
  dirs = os.listdir(pathIn)
  for dir in dirs:
    dirIn = pathIn + dir + '/'
    dirOut = pathOut + dir + '/'
    if not os.path.exists(dirOut):
      os.mkdir(dirOut)
    fns = os.listdir(dirIn)
    for fn in fns:
      name = fn.split('.')[0]
      fnIn = dirIn + name + '.segment'
      fnOff = dirOut + name + '.off'
      fnSummary = dirOut + name + '.summary'
      cmd = 'qhull Qt FA s o TO %s > %s 2>&1 < %s' % (fnOff, fnSummary, fnIn)
      print cmd
      os.system(cmd)
  
