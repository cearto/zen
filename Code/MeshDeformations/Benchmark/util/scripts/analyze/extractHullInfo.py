import os, sys

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

path = '../../../analysis/raw-analysis/'

# function to extract info from a file
def extract(fn):
  f = open(fn, 'r')
  lines = f.readlines()
  n = len(lines)
  lineArea = lines[n-3]
  lineVol = lines[n-2]
  #print lineArea
  #print lineVol
  results = [0, 0]
  words = lineArea.split()
  results[0] = float(words[len(words)-1])
  words = lineVol.split()
  results[1] = float(words[len(words)-1])
  return results

# function to judge whether a hull info file is valid
def hasHull(fn):
  f = open(fn, 'r')
  lines = f.readlines()
  f.close()
  return len(lines) <> 0

# function to count the number of files in directory dir that end with suffix
def countFiles(dir, suffix):
  count = 0
  for fn in os.listdir(dir):
    words = fn.split('.')
    if words[len(words)-1] == suffix:
     count += 1
  return count 

# extract convex hull information
for a in algorithms:
  print 'Processing convex hull info for %s...' % a
  path1 = path + a + '/qhull/'
  names = os.listdir(path1)
  for name in names:
    print name
    dir = path1 + name + '/'
    nseg = countFiles(dir, 'summary')
    fnOut = dir + 'AreaVol.txt'
    fOut = open(fnOut, 'w')
    for i in range(0, nseg):
      fnOff = dir + str(i) + '.off'
      if not os.path.exists(fnOff):
        continue
      if(hasHull(fnOff)):
        fnSummary = dir + str(i) + '.summary'
        [area, vol] = extract(fnSummary)
        fOut.write('%d\t%f\t%f\n' % (i, area, vol))
      else:
        fOut.write('%d\t%f\t%f\n' % (i, -1, -1))
    fOut.close()

