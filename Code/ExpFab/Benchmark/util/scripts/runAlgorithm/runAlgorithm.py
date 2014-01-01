# import modules and configurations
import os, sys
from config import *
includepath = os.getcwd() + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *

# set directories
root = os.getcwd()
dirMeshIn = root + '/../../../data/%s/' % Mesh_Format
dirTmp = root + '/../../../data/tmp/'
dirOut = dirTmp + name + '/'
if not os.path.exists(dirTmp):
  os.mkdir(dirTmp)
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
tool = root + '/' + tool
if not os.path.exists(tool):
  print 'Cannot find your segmentation program in %s' % tool
  sys.exit()

# read in numSegments if takeNSegment = 'True'
if takeNSegment:
  fnPara = root + '/../../parameters/nSegDiff.txt'
  f = open(fnPara, 'r')
  lines = f.readlines()
  assert(len(lines) == 400)
  para = []
  for i in range(0, 400):
    p = []
    line = lines[i]
    line = line.split()
    for w in line:
      p.append(int(w))
    para.append(p)

# run your algorithm
if takeNSegment:
  for i in fileIdRange:
    d1 = dirOut + str(i) + '/'
    if not os.path.exists(d1):
      os.mkdir(d1)
    fnMeshIn = dirMeshIn + str(i) + '.' + Mesh_Format
    p = para[i-1] 
    for nseg in p:
      fnMeshOut = d1 + str(i) + '_' + str(nseg) + '.' + Mesh_Format
      fnSegOut = d1 + str(i) + '_' + str(nseg) + '.seg'
      cmd = '%s %s %s %s %s %d %s' % (tool, fnMeshIn, fnMeshOut, fnSegOut, optionNSegment, nseg, options)
      print 'segment model %d with numSegment %d' % (i, nseg)
      os.system(cmd)
else:
  for i in fileIdRange:
    fnMeshIn = dirMeshIn + str(i) + '.' + Mesh_Format
    fnMeshOut = dirOut + str(i) + '.' + Mesh_Format
    fnSegOut = dirOut + str(i) + '.seg'
    cmd = '%s %s %s %s %s' % (tool, fnMeshIn, fnMeshOut, fnSegOut, options)
    print 'segment model %d' % i
    os.system(cmd)

#####################################
# pre-process data in the following #
#####################################

# set directories
dirMesh = root + '/../../../data/%s/' % Mesh_Format
dirIn = root + '/../../../data/tmp/%s/' % name
dirOut = root + '/../../../data/seg/%s/' % name
processTool = root + '/../../../exe/segPreprocess'
if os.name == 'nt':
  processTool += '.exe'
if (not os.path.exists(dirIn)) or (len(os.listdir(dirIn)) == 0):
  print 'Cannot find your segmentation results in %s' % dirIn
  sys.exit()
if not os.path.exists(processTool):
  print 'Cannot find the tool to preprocess segmentations in %s ' % processTool
  sys.exit()
if not os.path.exists(dirOut):
  os.mkdir(dirOut)

# process the segmentations
if takeNSegment:
  for i in fileIdRange:
    dI = dirIn + str(i) + '/'
    if not os.path.exists(dI):
      print 'Cannot find segmentations for model %d' % i
      continue
    dO = dirOut + str(i) + '/' 
    if not os.path.exists(dO):
      os.mkdir(dO)
    p = para[i-1] 
    mshSource = dirMesh + str(i) + '.%s' % Mesh_Format
    for nseg in p:
      mshInput = dI + str(i) + '_' + str(nseg) + '.' + Mesh_Format
      segInput = dI + str(i) + '_' + str(nseg) + '.seg'
      if not (os.path.exists(mshInput) and os.path.exists(segInput)):
        print 'Missing segmentations %d_%d' % (i, nseg)
        continue
      segOutput = dO + str(i) + '_' + str(nseg) + '.seg'  
      cmd = '%s %s %s %s %s' % (processTool, mshInput, segInput, segOutput, mshSource)
      print 'process segmentation %d_%d' % (i, nseg)
      os.system(cmd)
else:
  for i in fileIdRange:
    mshInput = dirIn + str(i) + '.' + Mesh_Format
    segInput = dirIn + str(i) + '.seg'
    segOutput = dirOut + str(i) + '.seg'
    mshSource = dirMesh + str(i) + '.%s' % Mesh_Format
    if not (os.path.exists(mshInput) and os.path.exists(segInput)):
      print 'Missing segmentations %d' % (i)
      continue
    cmd = '%s %s %s %s %s' % (processTool, mshInput, segInput, segOutput, mshSource)
    print 'process segmentation %d' % (i)
    os.system(cmd)

