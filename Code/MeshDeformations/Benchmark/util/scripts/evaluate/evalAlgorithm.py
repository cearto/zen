import os, sys

# set directories
root = os.getcwd()
dirPara = root + '/../../parameters/'
pathIn = root + '/../../../data/seg/'
pathEval = root + '/../../../evaluation/'
pathOut = pathEval + 'eval-raw/'
pathRef = root + '/../../../data/seg/Benchmark/'
tool = root + '/../../../exe/segEval '
toolB = root + '/../../../exe/groupSegEval ' # the one for benchmark

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
# those algorithms that take numSegment as input 
algorithms1 = []
# those algorithms that do *not* take numSegment as input
algorithms2 = []
# the benchmark
algorithms3 = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].needEvaluation == False): # only evaluate if needEvaluation = True
    continue
  if(allAlgorithms[i].AlgorithmType == 1):
    algorithms1.append(allAlgorithms[i].FullName)
  elif(allAlgorithms[i].AlgorithmType == 2):
    algorithms2.append(allAlgorithms[i].FullName)
  else:
    algorithms3.append(allAlgorithms[i].FullName) 

# different ways of setting number of segments
nSegModes = [
'BySegmentation', 'ByModel', 'ByCategory', 'ByDataset'
]

if not os.path.exists(pathEval):
  os.mkdir(pathEval)
if not os.path.exists(pathOut):
  os.mkdir(pathOut)

# functions to read in numSegment parameters
def readPara(m, dirPara):
  fn = dirPara + 'nSeg-%s.txt' % m
  f = open(fn, 'r')
  lines = f.readlines()
  f.close()
  para = []
  assert(len(lines) == 400)
  for line in lines:
    words = line.split()
    p = []
    for w in words:
      p.append(int(w))
    para.append(p)
  return para

# functions to evaluate multiple segmentations for one object to the benchmark(BySegmentation)
def evalMany2Many(algorithm, mode, para, pIn, pOut, pRef):
   pathIn = pIn + algorithm +'/'
   pathOut = pOut + algorithm + '-' + mode + '/'
   if not os.path.exists(pathOut):
     os.mkdir(pathOut)
   for i in fileIdRange:
     p = para[i-1]
     fnMesh = dirMesh + str(i) + '.%s' % Mesh_Format
     dirIn = pathIn + str(i) + '/'
     dirOut = pathOut + str(i) + '/'
     dirRef = pRef + str(i) + '/'
     if not os.path.exists(dirOut):
       os.mkdir(dirOut)
     for j in range(0, len(p)):
       fnIn = dirIn + str(i) + '_' + str(p[j]) + '.seg' 
       fnRef = dirRef + str(i) + '_' + str(j) + '.seg'
       cmd = '%s %s %s %s %s' % (tool, fnMesh, fnIn, fnRef, dirOut)
       #print cmd
       print "Evaluate %8s %10s %3d %2d" % (algorithm, mode, i, j)	
       os.system(cmd)

# functions to evalution only one segmentation for one object to the benchmark
def evalOne2Many(algorithm, mode, para, pIn, pOut, pRef):
  pathIn = pIn + algorithm + '/'
  if mode == '':
    pathOut = pOut + algorithm + '/'
  else:
    pathOut = pOut + algorithm + '-' + mode + '/'
  if not os.path.exists(pathOut):
    os.mkdir(pathOut)
  for i in fileIdRange:
    fnMesh = dirMesh + str(i) + '.%s' % Mesh_Format
    dirOut = pathOut + str(i) + '/'
    dirRef = pRef + str(i) + '/'
    if not os.path.exists(dirOut):
      os.mkdir(dirOut)
    if para == '':
      fnIn = pathIn + str(i) + '.seg'
    else:
      p = para[i-1][0]
      fnIn = pathIn + str(i) + '/' + str(i) + '_' + str(p) + '.seg'
    for j in range(0, len(os.listdir(dirRef))):   
      fnRef = dirRef + str(i) + '_' + str(j) + '.seg'
      cmd = '%s %s %s %s %s' % (tool, fnMesh, fnIn, fnRef, dirOut)
      #print cmd
      print "Evaluate %8s %10s %3d %2d" % (algorithm, mode, i, j)
      os.system(cmd)

# function to check dirIn has file from id_0.seg to id_(n-1).seg
def check(dirIn, id, n):
    for i in range(0, n):
        fn = dirIn + str(id) + '_' + str(i) + '.seg'
        if not os.path.exists(fn):
            return False
    return True

# function to evaluate the benchmark
def evalBenchmark(algorithm, pIn, pOut, pRef):
  print "Evaluating the benchmark"
  pathIn = pIn + algorithm + '/'
  pathOut = pOut + algorithm + '/'
  if not os.path.exists(pathOut):
    os.mkdir(pathOut)
  for i in fileIdRange:
    dirIn = pathIn + str(i) + '/'
    n = len(os.listdir(dirIn))
    if check(dirIn, i, n):
      print "Evaluate %3d" % i
      fnMesh = dirMesh + str(i) + '.%s' % Mesh_Format
      dirOut = pathOut + str(i) + '/'
      if not os.path.exists(dirOut):
        os.mkdir(dirOut)
      cmd = '%s %s %s %s %d %d' % (toolB, fnMesh, dirIn, dirOut, i, n)
      #print cmd
      os.system(cmd)
  

# main part, evaluation goes here
for m in nSegModes:
  para = readPara(m, dirPara)
  for a in algorithms1:
    if m == 'BySegmentation':
      evalMany2Many(a, m, para, pathIn, pathOut, pathRef)
    else: 
      evalOne2Many(a, m, para, pathIn, pathOut, pathRef)
for a in algorithms2:
  evalOne2Many(a, '', '', pathIn, pathOut, pathRef)
for a in algorithms3:
  evalBenchmark(a, pathIn, pathOut, pathRef)

