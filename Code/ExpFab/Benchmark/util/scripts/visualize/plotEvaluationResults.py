import os, sys

# set directories
path = '../../../evaluation/'
dirIn = path + 'eval-processed/stat/'
dirOut = path + 'plots/'
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
# different ways of setting number of segments
nSegModes = [
'BySegmentation', 'ByModel', 'ByCategory', 'ByDataset'
]
# suffix for different evaluations
suffix = [
'CD', 'CE', 'HD','RI'
]

# generate the algorithm list to be put in the matlab script
# include algorithms
includepath = os.getcwd() + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *
line = 'algorithms={'
for i in range(0, len(allAlgorithms)):
  if i == 0:
    line += '{\'%s\',\'%s\'}' % (allAlgorithms[i].SplittedName1,allAlgorithms[i].SplittedName2) 
  else:
    line += ',{\'%s\',\'%s\'}' % (allAlgorithms[i].SplittedName1,allAlgorithms[i].SplittedName2) 
line += '};\n'
line2 = 'mask=['
for i in range(0, len(allAlgorithms)):
  mask = 0
  if(allAlgorithms[i].AlgorithmType == 1):
    mask = 1
  if i == 0:
    line2 += '%d' % mask
  else:
    line2 += ',%d' % mask
line2 += '];\n'

# generate the commands to plot all  
dirOut1 = "\'" + dirOut + "\'"
cmd = ''
for m in nSegModes:
  mode = "\'" + m + "\'"
  for i in range(0, len(suffix)):
    s = "\'" + suffix[i] + "\'"
    fnIn = "\'" + dirIn + m + '.' + suffix[i] + "\'"  
    cmd += 'plotEvaluationStatistics(%s, %s, %s, %s, algorithms, %d);\n' % (fnIn, dirOut1, mode, s, 0)
    # if the last parameter is non-zero, evaluations of per-category level 
    # will be plotted

# a command to plot the results from different modes together 
fnOut = dirOut + 'RIByLevel' # should not have suffix 
fnOut = "\'" + fnOut + "\'"
dirIn1 = "\'" + dirIn + "\'"
cmd += 'plotRIByLevel(%s, %s, algorithms, mask);\n' % (dirIn1, fnOut)

# generate a batch file and run
f = open('test.m', 'w')
f.write(line)
f.write(line2)
f.write(cmd) 
f.close()
toRun = 'matlab -nodesktop -nosplash -r \"test; exit; \"'
os.system(toRun)

#################################################
# Create a webpage to show the generated images #
#################################################

# set directories
dirPara = '../../parameters/'
dirOut = '../../../evaluation/report/'
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
fn = dirOut + 'report.html'

# evaluation items
items = ['Consistency Error', 'Hamming Distance', 'Cut Discrepancy', 'Rand Index']
acronyms = ['CE', 'HD', 'CD', 'RI']

# create the html file
f = open(fn, 'w')
title = 'Evaluation Results'
f.write('<html><head><title>%s</title></head><body>\n' % title)
f.write('<h2>Rand Index for four levels</h2>\n')
fnImg = '../plots/RIByLevel.jpg'
f.write('<img  width=496 height=337 src="%s"></ BR>\n' % (fnImg))
f.write('<h2>Evaluation Results by Different metrics</h2>\n')
f.write('<table style="text-align: center" border="0" cellpadding="0" cellspacing="0"><tbody>\n')
interval = 2
count = 0
for i in range(0,len(items)):
  if count == 0:
    f.write('<tr>\n')
  count += 1
  fnImg = '../plots/ByModel-%s-all.jpg' % acronyms[i]
  f.write('<td><img width=496 height=337 src="%s"/><br><div style="text-align:center">%s</div></td>\n' % (fnImg,items[i]))
  if count == interval:
    count = 0
    f.write('</tr>\n')
if count <> 0:
  f.write('</tr>\n')
f.write('</tbody></table><br>\n')
f.write('</body></html>')
f.close()
print 'You can view the generated images now in %s' % fn

