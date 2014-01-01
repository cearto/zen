import os, sys, math

# set directories
path = '../../../analysis/'
pathPlots = path + 'plots/'
if not os.path.exists(pathPlots):
  os.mkdir(pathPlots)

# include algorithms
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
  algorithms.append(allAlgorithms[i].FullName)

cmds = ''
for a in algorithms:
    aa = a
    dirIn = path + 'raw-analysis/' + a + '/stat-processed/'
    dirOut = pathPlots + a + '/'
    if not os.path.exists(dirOut):
        os.mkdir(dirOut)
    aa = "\'"+aa+"\'"
    dirIn = "\'"+dirIn+"\'"
    dirOut = "\'"+dirOut+"\'"
    cmds += 'plotAnalysisResults(%s,%s,%s,%d);\n' % (aa,dirIn,dirOut,1)

f = open('test.m', 'w')
f.write(cmds)
f.close()
cmd = 'matlab -nodesktop -nosplash -r \"test; exit; \"'
#print cmd
os.system(cmd)

############################################
# Create a webpage to show generated plots #
############################################

# set directories
dirPara = '../../parameters/'
pathPlots = '../../../analysis/plots/'
dirOut = '../../../analysis/report/'
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
fn = dirOut + 'report.html'

# properties
properties = [
'angle', 	'area', 	'areaRatio', 	'curv-gauss', 	'curv-max',
'curv-mean', 	'curv-min', 	'curv-valley1', 'curv-valley',	'length',
'lengthRatio',	'convexity',	'compactness',	'nseg',		'nseg-unfiltered',
]
nProperties = len(properties)
toDraw = [ # whether to show each property
True,		False,		True,		False,		False,
False,		True,		False,		True,		False,
True,		False,		False,		True,		False,
] 

# include algorithm list
includepath = os.getcwd() + '/../../parameters/'
sys.path.append(includepath)
from algorithmList import *
algorithms = []
for i in range(0, len(allAlgorithms)):
  algorithms.append(allAlgorithms[i].FullName)
nAlgorithms = len(algorithms)

# generating report.html
f = open(fn, 'w')
interval = 4
title = 'Analysis Results'
f.write('<html><head><title>%s</title></head><body>\n' % title)
for i in range(0, nProperties):
  if not toDraw[i]:
    continue
  count = 0
  name = properties[i]
  f.write('<h2>%s</h2>\n' % name)
  f.write('<table style="text-align:left" border=0 cellspace=0 cellpadding=0><tbody>\n')
  for j in range(0,nAlgorithms):
    if count == 0:
      f.write('<tr>\n')
    count += 1
    fnImg = '../plots/%s/%s.jpg' % (algorithms[j], properties[i])
    content = '<img style="width:256px;height:210px;" src="%s">' % fnImg
    if algorithms[j] <> 'Benchmark':
      content += '<div style="text-align:center">%s</div>' % algorithms[j]
    else: # Benchmark
      content += '<div style="text-align:center"><span style="color:rgb(255,0,0)">%s</div>' % algorithms[j]
    f.write('<td>%s</td>\n' % content)
    if count == interval:
      f.write('</tr>\n')
      count = 0
  if count <> 0:
    f.write('</tr>\n')
  f.write('</tbody></table></br>\n')

f.write('</body></html>')
f.close()

print "You can view the analysis plot now in %s" % fn
