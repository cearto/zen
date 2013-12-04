import os, sys 

# set directories
root = os.getcwd()
pathAnalysis = root + '/../../../analysis/'
pathIn = root + '/../../../analysis/seg/'
pathInBenchmark = root + '/../../../data/seg/Benchmark/'
pathOut = root + '/../../../analysis/raw-analysis/'
dirCurv = root + '/../../../data/curvature/'
tool = root + '/../../../exe/segAnalysis '
if not os.path.exists(pathAnalysis):
  os.mkdir(pathAnalysis)
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

# analyze segmentions for model i 
def analyze(i, pathIn, dirStat, pathHull, forBenchmark):
  if forBenchmark:
    dirIn = pathInBenchmark + str(i) + '/'
    n = len(os.listdir(dirIn))
  else:
    dirIn = pathIn
    n = 1
    fn = pathIn + str(i) + '_0.seg'
    if not os.path.exists(fn):
      return 
  fnMesh = dirMesh + str(i) + '.%s' % Mesh_Format
  fnCurv = dirCurv + str(i) + '.curv'
  cmd = '%s %s -id %d -nFilesIn %d -dirStat %s -dirIn %s ' % (tool, fnMesh, i, n, dirStat, dirIn)
  # as below are properties to be analyzed
  # comment out if you do not want to analyze the related properties 
  cmd += '-area '
  cmd += '-length '
  cmd += '-angle '
  if os.path.exists(fnCurv):
    cmd += '-curv %s ' % fnCurv
  else:
    errMsg = "Cannot find curvature information at %s, " % fnCurv
    errMsg += "please comment out the lines in function analyze(..) for curvatures in analyzeAlgorithms.py"
    print errMsg
  cmd += '-numSegments '
  #cmd += '-convexity %s' % pathHull
  print cmd
  os.system(cmd)

# analyze every algorithm
for a in algorithms:
  print 'Analyze %s' % a
  pIn = pathIn + a + '/'
  pOut = pathOut + a + '/'
  if not os.path.exists(pOut):
    os.mkdir(pOut)
  pStat = pOut + 'stat-raw/'
  if not os.path.exists(pStat):
    os.mkdir(pStat)
  pHull = pathOut + a + '/qhull/'
  forBenchmark = False
  if a == 'Benchmark':
    forBenchmark = True
  for i in fileIdRange:
    analyze(i, pIn, pStat, pHull, forBenchmark)

