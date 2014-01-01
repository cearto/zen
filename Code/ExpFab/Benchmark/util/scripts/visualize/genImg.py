import os, sys

# set directories
root = os.getcwd()
pathSeg = root + '/../../../data/seg/'
pathImg = root + '/../../../data/img/'
tool = root + '/../../../exe/mshview'
fnCamera = root + '/../../parameters/camera.txt'
if not os.path.exists(pathImg):
  os.mkdir(pathImg)

# include algorithms
includepath = root + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *
dirMesh = root + '/../../../data/%s/' % Mesh_Format
# those algorithms having multiple segmentations per model(including Benchmark)   
algorithms1 = []
# those algorithms *not* having multiple segmentations per model 
algorithms2 = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].needColoredImage == False):
    continue
  if(allAlgorithms[i].AlgorithmType == 1 or allAlgorithms[i].AlgorithmType == 3):
    algorithms1.append(allAlgorithms[i].FullName)
  else:
    algorithms2.append(allAlgorithms[i].FullName)


# read in camera parameters
f = open(fnCamera, 'r')
camera = []
lines = f.readlines()
f.close()
for line in lines:
  p = line.split()
  camera.append(p) 

# function to take snapshots of one segmentation
def snapshotOne(fnMesh, fnSeg, fnImg, pCamera):
   cmd = '%s %s -seg %s -image %s -segments -camera' % (tool, fnMesh, fnSeg, fnImg)
   for j in range(0, 9):
     cmd += ' ' + pCamera[j]
   #print cmd
   os.system(cmd)


# function to take snapshots of segmentations from an algorithm
def snapshot(algorithm, camera, hasMultiPerModel):
  print 'snapshot %s' % algorithm
  dirImg = pathImg + algorithm + '/'
  if not os.path.exists(dirImg):
    os.mkdir(dirImg)
  dirSeg = pathSeg + algorithm + '/'
  if hasMultiPerModel: # algorithms1
    for i in fileIdRange:
      dirImg1 = dirImg + str(i) + '/'
      if not os.path.exists(dirImg1):
        os.mkdir(dirImg1)
      dirSeg1 = dirSeg + str(i) + '/'
      fnMesh = dirMesh + str(i) + '.%s' % Mesh_Format
      fns = os.listdir(dirSeg1)
      pCamera = camera[i-1]
      for fn in fns:
        name = fn.split('.')[0]
        fnImg = dirImg1 + name + '.bmp'  
        fnSeg = dirSeg1 + fn
        snapshotOne(fnMesh, fnSeg, fnImg, pCamera) 
  else: # algorithms2
    fns = os.listdir(dirSeg)
    for fn in fns:
      name = fn.split('.')[0]
      fnImg = dirImg + name + '.bmp'
      fnSeg = dirSeg + fn
      fnMesh = dirMesh + name + '.%s' % Mesh_Format
      pCamera = camera[int(name)-1]
      snapshotOne(fnMesh, fnSeg, fnImg, pCamera)

# get colored images for all segmentations
for a in algorithms1:
  snapshot(a, camera, True)
for a in algorithms2:
  snapshot(a, camera, False)
