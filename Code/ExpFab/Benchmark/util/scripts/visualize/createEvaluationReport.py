import os, sys

dirPara = '../../parameters/'
dirOut = '../../../evaluation/report/'
# create report directory and copy files
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
cmd = 'cp %s %s' % (dirPara + 'Makefile.evalReport', dirOut + 'Makefile')
os.system(cmd)
fn = dirOut + 'report.tex'

# evaluation items
items = ['Consistency Error', 'Hamming Distance', 'Cut Discrepancy', 'Rand Index']
acronyms = ['CE', 'HD', 'CD', 'RI']

# include algorithms
includepath = os.getcwd() + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *
algorithms1 = []
algorithms2 = []
for i in range(0, len(allAlgorithms)):
  if allAlgorithms[i].AlgorithmType<>3:
    algorithms1.append(allAlgorithms[i].SplittedName1)
    algorithms2.append(allAlgorithms[i].SplittedName2)

# create the tex file
f = open(fn, 'w')
f.write('%s\n' % '\\documentclass{article}')
f.write('%s\n' % '\\usepackage{graphicx}')
f.write('%s\n' % '\\usepackage{color}')
f.write('%s\n' % '\\usepackage{subfigure}')
f.write('%s\n' % '')
f.write('%s\n' % '\\begin{document}')
f.write('%s\n' % '')
f.write('%s\n' % '\\begin{figure}')
f.write('%s\n' % '\\centering')
fnImg = '../plots/RIByLevel.eps'
f.write('\\includegraphics[width=0.49\\textwidth]{%s}\n' % fnImg)
f.write('%s\n' % '\\caption{Rand Index for four levels}')
f.write('%s\n' % '\\end{figure}')
f.write('%s\n' % '')
f.write('%s\n' % '\\begin{table}\\footnotesize')
f.write('%s\n' % '\\begin{tabular}{|l|ccccccc|}')
f.write('%s\n' % '  \\hline')
s = '  Object'
for i in range(0, len(algorithms1)):
  s += '& %s ' % algorithms1[i]
s += '\\\\'
f.write('%s\n' % s)
s = '  Categories'
for i in range(0, len(algorithms2)):
  s += '& %s ' % algorithms2[i]
s += '\\\\'
f.write('%s\n' % s)
f.write('%s\n' % '  \\hline')
fIn = open('../../../evaluation/plots/ByModel-RI-rank.txt','r')
lines = fIn.readlines()
fIn.close()
for i in range(0, len(categories)):
  if i == 13:
    continue
  s = '  %s ' % categories[i]
  words = lines[i+1].split()
  for w in words:
    rank = int(w)
    if rank == 1:
      s += '& \\textcolor{red}{' + str(rank) + '}'
    else:
      s += '& ' + str(rank)
  s += '\\\\\n'
  f.write(s)
f.write('%s\n' % '  \\hline')
s = '  Overall '
words = lines[0].split()
for w in words:
  rank = int(float(w))
  if rank == 1:
    s += '& \\textcolor{red}{' + str(rank) + '}'
  else:
    s += '& ' + str(rank)
s += '\\\\'
f.write('%s\n' % s)
f.write('%s\n' % '\\hline')
f.write('%s\n' % '\\end{tabular}')
f.write('\\caption{Comparison of segmentation algorithms for each object category. Entries represent the rank of the algorithm according to the Rand Index evaluation metric (1 is the best, and %d is the worst).}\n' % len(algorithms1))
f.write('%s\n' % '\\end{table}')
f.write('%s\n' % '')
f.write('%s\n' % '\\begin{figure*}[h]')
f.write('%s\n' % '\\centering')
for i in range(0,len(items)):
  fnImg = '../plots/ByModel-%s-all.eps' % acronyms[i]
  f.write('\\subfigure[%s]{\\includegraphics[width=0.49\\textwidth]{%s}}\n' % (items[i], fnImg))
  if i == 1:
    f.write('\\hfill\n')
f.write('%s\n' % '\\caption{Evaluation results from ByModel}')
f.write('%s\n' % '\\end{figure*}')
f.write('%s\n' % '\\newpage')
f.write('%s\n' % '')
f.write('%s\n' % '\\end{document}')
f.close()

# make report
os.chdir(dirOut)
os.system('make')

