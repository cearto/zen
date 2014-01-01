##################################
## Import & Setting Parameters	## 
##################################
import os, sys
root = os.getcwd()
includepath = root + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *

# different ways of setting number of segments
nSegModes = [
'BySegmentation', 'ByModel', 'ByCategory', 'ByDataset'
]
# suffix for different evaluation metrics
suffix = ['CD', 'CE', 'HD', 'RI']
# number of items per evaluation metrics
nItems = [1, 4, 3, 1]
# names of the items for each evaluation metrics
nameItems = [
['CD'],					# Cut Discrepancy
['GCEf', 'LCEf', 'GCE', 'LCE'],		# Consistency Error, first two are numFace weighted, second two are area weighted 
['Hamming', 'Hamming-Rm', 'Hamming-Rf'],# Hamming Distance, the second is the missing rate, the third is the false alarm rate
['RI']					# Rand Index	
]
# whether to show some debugging info
verbose = False
 

##################################
## Collect Evaluation Results 	##
##################################

# set directories
path = root + '/../../../evaluation/eval-raw/'
dirPara = root + '/../../parameters/'

# those algorithms that take numSegment as input 
algorithms1 = []
# those algorithms that do *not* take numSegment as input
algorithms2 = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].needEvaluation == False):
    continue 
  if(allAlgorithms[i].AlgorithmType == 1):
    algorithms1.append(allAlgorithms[i].FullName)
  elif(allAlgorithms[i].AlgorithmType == 2):
    algorithms2.append(allAlgorithms[i].FullName)

if verbose:
  print "stage 1"
  print algorithms1
  print algorithms2

# read in nSeg-BySegmentation.txt, to know how many segmentations for each model
f = open(dirPara + 'nSeg-BySegmentation.txt', 'r')
lines = f.readlines()
f.close()
assert(len(lines) == 400)
nSegmentations = []
for line in lines:
  words = line.split()
  nSegmentations.append(len(words))

# function to collect evaluation results 
def collect(algorithm, mode, suffix, nSegmentations, path):
  print 'collecting %s %s %s ...' % (algorithm, mode, suffix)
  if mode == '':
    dir = path + algorithm + '/'
  else:
    dir = path + algorithm + '-' + mode + '/'
  for i in fileIdRange:
    n = nSegmentations[i-1]
    data = []
    for j in range(0,n):
      fn = dir + str(i) + '/' + str(i) + '_' + str(j) + '.' + suffix
      if os.path.exists(fn):
        f = open(fn)
        line = f.readline()
        data.append(line)
        f.close()
    if len(data) <> 0:
      fn = dir + str(i) + '/' + str(i) + '.' + suffix
      f = open(fn, 'w')
      for d in data:
        f.write(d)
      f.close()
  print "Done with collecting %s %s %s" % (algorithm, mode, suffix)

# collect evaluation results
for a in algorithms1:
  for m in nSegModes:
    for s in suffix:
      collect(a, m, s, nSegmentations, path) 
for a in algorithms2:
  for s in suffix:
    collect(a, '', s, nSegmentations, path)


#######################################
## Go on with the process precedure  ##
#######################################

# set directories
pathIn = root + '/../../../evaluation/eval-raw/'
pathOut = root + '/../../../evaluation/eval-processed/'
if not os.path.exists(pathOut):
  os.mkdir(pathOut)

# those algorithms taking numSegment as inputs 
algorithms1 = []
# those algorithms *not* taking numSegment as inputs (including Benchmark)
algorithms2 = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].needEvaluation == False):
    continue
  if(allAlgorithms[i].AlgorithmType == 1):
    algorithms1.append(allAlgorithms[i].FullName)
  else:
    algorithms2.append(allAlgorithms[i].FullName)

if verbose:
  print "stage 2"
  print algorithms1
  print algorithms2

# function to get mean values from a matrix
def getMean(matrix):
  m = len(matrix)
  n = len(matrix[0]) # it is an m by n matrix
  for i in range(1, m):
    for j in range(0, n):
      matrix[0][j] += matrix[i][j]
  for j in range(0,n):
    matrix[0][j] /= m
  return matrix[0]  

# function to process evaluation results
def process(algorithm, mode, suffix, pathIn, pathOut):
  print "processing %s %s %s" % (algorithm, mode, suffix)
  if mode == '':
    name = algorithm
  else:
    name = algorithm + '-' + mode
  dirIn = pathIn + name + '/'
  dirOut = pathOut + name + '/'
  if not os.path.exists(dirOut):
    os.mkdir(dirOut)
  fnOut = dirOut + name + '.' + suffix
  fOut = open(fnOut, 'w')
  for i in fileIdRange:
    fnIn = dirIn + str(i) + '/' + str(i) + '.' + suffix
    if not os.path.exists(fnIn):
      continue
    fIn = open(fnIn, 'r')
    lines = fIn.readlines()
    fIn.close()
    data = []
    for line in lines:
      words = line.split()
      d = []
      for w in words:
        d.append(float(w))
      data.append(d)
    mean = getMean(data)
    fOut.write('%d\t' % i)
    for m in mean:
      fOut.write('%f\t' % m)
    fOut.write('\n')
  fOut.close()
  print "Done with processing %s %s %s" % (algorithm, mode, suffix) 

# process evaluation results   
for a in algorithms1:
  for m in nSegModes:
    for s in suffix:
      process(a, m, s, pathIn, pathOut)
for a in algorithms2:
  for s in suffix:
    process(a, '', s, pathIn, pathOut)


##########################################
## Get the final evaluation statistics  ##
##########################################

# set directories
path = root + '/../../../evaluation/eval-processed/'
dirOut = path + 'stat/'
if not os.path.exists(dirOut):
  os.mkdir(dirOut)

# include algorithm list
algorithms = []
mask = []
for i in range(0, len(allAlgorithms)):
  algorithms.append(allAlgorithms[i].FullName)
  if allAlgorithms[i].AlgorithmType == 1:
    mask.append(True)
  else:
    mask.append(False)

if verbose:
  print "stage 3"
  print algorithms
  print mask

# function to get statistics for one algorithm
def getOneStatistics(fn, n):
  f = open(fn, 'r')
  lines = f.readlines()
  f.close()
  m = []
  count = []
  # we assume no Numpy is installed for a simpler implementation 
  for i in range(0, 21): 
  # the first row for average of all models, 
  # the other 20 rows for average of each category (category 14 is not used) 
    mm = []
    count.append(0)
    for j in range(0, n):
      mm.append(0)
    m.append(mm) 
  for line in lines:
    words = line.split()
    id = int(words[0])
    data = words[1:len(words)]
    count[0] += 1
    c = int((id-1)/20) + 1 # category 
    count[c] += 1
    for i in range(0, n):
      m[0][i] += float(data[i])
      m[c][i] += float(data[i])  
  # get the average
  for i in range(0, 21):
    if count[i] == 0:
      continue
    for j in range(0, n):
      m[i][j] /= count[i]
  return m
  
# function to get all statistics 
def getStatistics(mode, indexSuffix):
  s = suffix[indexSuffix] # suffix
  n = nItems[indexSuffix] # number of evaluation items given the suffix
  print 'get statistics for %s %s' % (mode, s)
  data = []
  for i in range(0, len(algorithms)):
    a = algorithms[i]
    if mask[i]:
      a += '-' + mode
    fn = path + a + '/' + a + '.' + s
    m = getOneStatistics(fn, n)
    data.append(m)
  ##########################
  # Output the raw results #
  ##########################
  fn = dirOut + mode + '.' + s
  f = open(fn, 'w')
  for i in range(0, 21): 
  # the first row is the average of the whole dataset
  # the next 20 are the averages of each category (category 14 is not used)
    for j in range(0, len(algorithms)):
      dd = data[j][i]
      for d in dd:
        f.write('%f\t' % d)
    f.write('\n')
  f.close()
  ##########################
  # Output to an easy to   #
  # read spreadsheet       #
  ##########################
  fn = dirOut + mode + '.' + s + '.xls'
  f = open(fn, 'w')
  # make head line
  headline = '\t'
  for i in range(0, len(algorithms)):
    headline += '%s\t' % algorithms[i]
    for j in range(0, n-1):
      headline += '\t'
  headline += '\n'
  f.write(headline)
  # make second head line
  headline = '\t'
  arr = ''
  for i in range(0, n):
    arr += '%s\t' % nameItems[indexSuffix][i]
  for i in range(0, len(algorithms)):
    headline += arr
  headline += '\n'
  f.write(headline)
  # write in the result
  list = ['Average'] + categories
  for i in range(0, 21):
    if i == 14: # skip category spring
      continue
    line = list[i] + '\t'
    for j in range(0, len(algorithms)):
      dd = data[j][i]
      for d in dd:
        line += ('%f\t' % d)  
    line += '\n'
    f.write(line)
  f.close()

# get all statistics
for m in nSegModes:
  for i in range(0, len(suffix)):
    getStatistics(m, i)



