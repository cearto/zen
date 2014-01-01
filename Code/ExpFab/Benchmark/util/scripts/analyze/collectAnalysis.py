import os, sys, math

# set path
if os.name == 'nt':
  SLASH = '\\'
else:
  SLASH = '/'
path = '..' + SLASH + '..' + SLASH + '..' + SLASH + 'analysis' + SLASH + 'raw-analysis' + SLASH

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
  if (allAlgorithms[i].needAnalysis == False):
    continue
  algorithms.append(allAlgorithms[i].FullName)


def copy(dirIn, dirOut, item):
  fnOut = dirOut + 'all.' + item
  for i in fileIdRange:
    fnIn = dirIn + str(i) + '.' + item
    if os.path.exists(fnIn):
      if os.name == 'nt':
        cmd = 'type %s >> %s' % (fnIn, fnOut)
      else:
        cmd = 'cat %s >> %s' % (fnIn, fnOut) 
      os.system(cmd)

def discretizeOne(bin, dirIn, i, item):
  fnIn = dirIn + str(i) + '.' + item
  if os.path.exists(fnIn):
    fIn = open(fnIn, 'r')
    lines = fIn.readlines()
    for line in lines:
      line = float(line) * 180 / math.pi
      bin[int(math.floor(line+0.5))] += 1
    fIn.close()

def discretize(dirIn, dirOut, item):
  fnOut = dirOut + 'all.' + item
  fOut = open(fnOut, 'w')
  bin = range(0,361)
  for i in range(0, 361):
    bin[i] = 0
  for i in fileIdRange:
    discretizeOne(bin, dirIn, i, item)
  for i in range(0, 361):
    fOut.write(str(bin[i])+'\n')
  fOut.close()

def makeBin(ranges):
  low = ranges[0]
  high = ranges[1]
  bin = range(0, high-low+1)
  for i in range(0, high-low+1):
    bin[i] = 0;
  return bin

def outputBin(bin, ranges, dirOut, suffix):
  fnOut = dirOut + 'all.' + suffix
  fOut = open(fnOut, 'w')
  low = ranges[0]
  high = ranges[1]
  for i in range(low, high+1):
    fOut.write(str(i) + ' ' + str(bin[i-low]) + '\n')
  fOut.close()

def addToBin(element, bin, ranges):
  pos = int(math.floor(element+0.5)) - ranges[0]
  if (pos < 0):
    pos = 0
  elif pos > (ranges[1]-ranges[0]):
    pos = ranges[1] - ranges[0]
  bin[pos] = bin[pos] + 1

def collectCurvOne(bMin, rMin, bMax, rMax, bMean, rMean, bGauss, rGauss, bValley, rValley, bValley1, rValley1, dirIn, i, item):
  fnIn = dirIn + str(i) + '.' + item
  if not os.path.exists(fnIn):
    return
  fIn = open(fnIn, 'r')
  lines = fIn.readlines()
  fIn.close()
  for line in lines:
    words = line.split()
    c0 = float(words[0])
    c1 = float(words[1])
    dc0 = float(words[2])
    dc3 = float(words[5])
    minC = min(c0,c1)
    maxC = max(c0,c1)
    meanC = (c0+c1)/2
    gaussC = c0*c1
    valleyC = 0
    valleyC1 = 0
    if c0 < c1:
      valleyC = math.fabs(dc0)
      valleyC1 = dc0
    else:
      valleyC = math.fabs(dc3)
      valleyC1 = dc3
    addToBin(minC, bMin, rMin)
    addToBin(maxC, bMax, rMax)
    addToBin(meanC, bMean, rMean)
    addToBin(gaussC, bGauss, rGauss)
    if minC < 0: # only when minC <0 , it may be valley
      addToBin(valleyC, bValley, rValley)
      addToBin(valleyC1, bValley1, rValley1)

def collectCurv(dirIn, dirOut, item):
  # create bins
  rMin = [-200, 200]
  rMax = [-200, 200]
  rMean = [-400, 400]
  rGauss = [-10000, 10000]
  rValley = [0,5000]
  rValley1 = [-5000,5000]
  bMin = makeBin(rMin)
  bMax = makeBin(rMax)
  bMean = makeBin(rMean)
  bGauss = makeBin(rGauss)
  bValley = makeBin(rValley)
  bValley1 = makeBin(rValley1)
  # stat
  for i in fileIdRange:
    collectCurvOne(bMin, rMin, bMax, rMax, bMean, rMean, bGauss, rGauss, bValley, rValley, bValley1, rValley1, dirIn, i, item)
  # output
  outputBin(bMin, rMin, dirOut, item+'-min')
  outputBin(bMax, rMax, dirOut, item+'-max')
  outputBin(bMean, rMean, dirOut, item+'-mean')
  outputBin(bGauss, rGauss, dirOut, item+'-gauss')
  outputBin(bValley, rValley, dirOut, item+'-valley')
  outputBin(bValley1, rValley1, dirOut, item+'-valley1')

for a in algorithms:
  print 'Collecting analysis of %s' % a
  dirIn = path + a + SLASH + 'stat-raw' + SLASH
  dirOut = path + a + SLASH + 'stat-processed' + SLASH
  if not os.path.exists(dirOut):
    os.mkdir(dirOut)
  copy(dirIn, dirOut, 'length')
  copy(dirIn, dirOut, 'area')
  copy(dirIn, dirOut, 'lengthRatio')
  copy(dirIn, dirOut, 'areaRatio')
  discretize(dirIn, dirOut, 'angleNotOnCut')
  discretize(dirIn, dirOut, 'angleOnCut')
  collectCurv(dirIn, dirOut, 'curvOnCut')
  collectCurv(dirIn, dirOut, 'curvNotOnCut')
  copy(dirIn, dirOut, 'nseg')
  # this is for convexity
  # copy(dirIn, dirOut, 'convexity')

