import os, sys

# set directories and link commands according to OS
if os.name == 'nt': # windows
  dirPara = '..\\..\\parameters\\'
  pathIn = '..\\..\\..\\data\\seg\\'
  pathAnalysis = '..\\..\\..\\analysis'
  pathOut = '..\\..\\..\\analysis\\seg\\'
  SLASH = '\\'
  SOFTLINK = 'mklink /D '
  HARDLINK = 'mklink /H '
else:  # Linux
  dirPara = '../../parameters/'
  pathIn = '../../../data/seg/'
  pathAnalysis = '../../../analysis'
  pathOut = '../../../analysis/seg/'
  SLASH = '/'
  SOFTLINK = 'ln -s '
  HARDLINK = 'ln '
if not os.path.exists(pathAnalysis):
  print pathAnalysis
  os.mkdir(pathAnalysis)
if not os.path.exists(pathOut):
  print pathOut
  os.mkdir(pathOut)


# include algorithm list
includepath = os.getcwd() + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *
# those algorithms that take numSegment as input 
algorithms1 = []
# those algorithms that do *not* take numSegment as input
algorithms2 = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].needAnalysis == False):
    continue
  if(allAlgorithms[i].AlgorithmType == 1):
    algorithms1.append(allAlgorithms[i].FullName)
  elif(allAlgorithms[i].AlgorithmType == 2):
    algorithms2.append(allAlgorithms[i].FullName)


# read in parameters
f = open(dirPara + 'nSeg-ByModel.txt', 'r')
lines = f.readlines()
f.close()
assert(len(lines) == 400)
para = []
for line in lines:
  words = line.split()
  p = []
  for w in words:
    p.append(int(w))
  para.append(p)

# function to make links
def makeLink(cmd, target, linkSymbol):
  if os.name == 'nt':
    line = '%s %s %s' % (cmd, linkSymbol, target)
  else:
    line = '%s %s %s' % (cmd, target, linkSymbol)
  return line

# make links for algorithms in group1
for a in algorithms1:
  dirIn = pathIn + a + SLASH
  dirOut = pathOut + a + SLASH
  if not os.path.exists(dirOut):
    os.mkdir(dirOut)
  print 'making links for %s' % a 
  for i in fileIdRange:
    nseg = para[i-1][0]
    target = dirIn + str(i) + SLASH + str(i) + '_' + str(nseg) + '.seg'
    linkSymbol = dirOut + str(i) + '_0.seg'
    cmd = makeLink(HARDLINK, target, linkSymbol)
    #print cmd
    os.system(cmd)

# make links for algorithms in group2
for a in algorithms2:
  dirIn = pathIn + a + SLASH
  dirOut = pathOut + a + SLASH
  if not os.path.exists(dirOut):
    os.mkdir(dirOut)
  print 'making links for %s' % a
  for i in fileIdRange:
    target = dirIn + str(i) + '.seg'
    if not os.path.exists(target):
      continue
    linkSymbol = dirOut + str(i) + '_0.seg'
    cmd = makeLink(HARDLINK, target, linkSymbol)
    #print cmd
    os.system(cmd)


