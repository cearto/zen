import os, sys

# directories
dirPara = '../../parameters/'
pathPlots = '../../../analysis/plots/'
dirOut = '../../../analysis/report/'

# create the report directory and copy files
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
cmd = 'cp %s %s' % (dirPara + 'Makefile.evalReport', dirOut + 'Makefile')
os.system(cmd)
fn = dirOut + 'report.tex'

# properties
properties = [
'angle', 	'area', 	'areaRatio', 	'curv-gauss', 	'curv-max',
'curv-mean', 	'curv-min', 	'curv-valley1', 'curv-valley',	'length',
'lengthRatio',	'convexity',	'compactness',	'nseg',		'nseg-unfiltered',
]
nProperties = len(properties)
toDraw = [ # whether to draw each property
True,		False,		True,		False,		False,
False,		True,		False,		True,		False,
True,		False,		False,		True,		False,
] 

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
nAlgorithms = len(algorithms)

# generating report.tex
# create the tex file
f = open(fn, 'w')
f.write('%s\n' % '\\documentclass{article}')
f.write('%s\n' % '\\usepackage{graphicx}')
f.write('%s\n' % '\\usepackage{subfigure}')
f.write('%s\n' % '')
f.write('%s\n' % '\\begin{document}')
f.write('%s\n' % '')
interval = 4
width = 0.96 / interval
for i in range(0, nProperties):
  if not toDraw[i]:
    continue
  f.write('%s\n' % '\\begin{figure*}[h]')
  f.write('%s\n' % '\\centering')
  count = 0
  for j in range(0,nAlgorithms):
    count += 1
    fnImg = '../plots/%s/%s.eps' % (algorithms[j], properties[i])
    f.write('\\subfigure[%s]{\\includegraphics[width=%f\\textwidth]{%s}}\n' % (algorithms[j],width,fnImg))
    if count == interval:
      f.write('\\hfill\n')
      count = 0
  f.write('\\caption{Perperty %s}\n' % properties[i])
  f.write('%s\n' % '\\end{figure*}')
  #f.write('%s\n' % '\\newpage')
  f.write('%s\n' % '')
f.write('%s\n' % '\\end{document}')
f.close()

# make report
os.chdir(dirOut)
os.system('make')

