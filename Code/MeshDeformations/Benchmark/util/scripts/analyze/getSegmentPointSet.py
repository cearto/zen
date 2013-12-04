import os, sys

# set directories
root = os.getcwd()
pathIn = root + '/../../../analysis/seg/'
pathInBenchmark = root + '/../../../data/seg/'
pathOut = root + '/../../../analysis/raw-analysis/'
tool = root + '/../../../exe/outputSegments '
if not os.path.exists(pathOut):
  os.mkdir(pathOut)

# include algorithm list
includepath = root + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *
dirMesh = root + '/../../../data/%s/' % Mesh_Format
algorithms = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].needAnalysis == False):
    continue
  algorithms.append(allAlgorithms[i].FullName)

for a in algorithms:
  print 'Output segment point sets for %s' % a
  path1 = pathOut + a + '/'
  if not os.path.exists(path1):
    os.mkdir(path1)
  path1 += 'segments/'
  if not os.path.exists(path1):
    os.mkdir(path1)
  if a == 'Benchmark':
    for i in fileIdRange:
      fnMesh = dirMesh + str(i) + '.%s' % Mesh_Format
      dirIn = pathInBenchmark + 'Benchmark/' + str(i) + '/'
      fns = os.listdir(dirIn)
      for fn in fns:
        name = fn.split('.')[0]
        fnSeg = dirIn + name + '.seg'
        dirOut = path1 + name + '/'
        if not os.path.exists(dirOut):
          os.mkdir(dirOut)
        cmd = '%s %s -seg %s %s' % (tool, fnMesh, fnSeg, dirOut)
        #print cmd
        os.system(cmd)
  else:
    for i in fileIdRange:
      fnMesh = dirMesh + str(i) + '.%s' % Mesh_Format
      dirIn = pathIn + a + '/'
      name = str(i) + '_0'
      fnSeg = dirIn + name + '.seg'
      if not os.path.exists(fnSeg):
        continue
      dirOut = path1 + name + '/'
      if not os.path.exists(dirOut):
        os.mkdir(dirOut)
      cmd = '%s %s -seg %s %s' % (tool, fnMesh, fnSeg, dirOut)
      #print cmd
      os.system(cmd)  

