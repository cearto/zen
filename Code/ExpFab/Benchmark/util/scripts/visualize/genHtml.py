import os, sys
# the type of segmentations to be visualized
mode = 'ByModel' # 'ByCategory', 'ByDataset'

#####################
# set directories   #
#####################
root = os.getcwd()
includepath = root + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *
dirWWW = root + '/../../../data/www/' # predefined place to store the webpages
if not os.path.exists(dirWWW):
  os.mkdir(dirWWW)
if os.name <> 'nt':
  cmd = 'cp ../../parameters/style.css ../../../data/www'
else:
  cmd = 'copy ..\\..\\parameters\\style.css ..\\..\\..\\data\\www'
os.system(cmd)  
dirPara = root + '/../../parameters/'
dirSeg = root + '/../../../data/seg/'
# algorithms
algorithms = []
# benchmark
benchmark = []
for i in range(0, len(allAlgorithms)):
  if(allAlgorithms[i].AlgorithmType <> 3):
    algorithms.append(allAlgorithms[i])
  else:
    benchmark.append(allAlgorithms[i]) 

######################
# index.html	     #	
######################
print "Generating index.html"
fn = dirWWW + 'index.html'
f = open(fn, 'w')
title = 'Colored Images of Segmentations'
interval = 5
f.write('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">\n')
f.write('<html xmlns="http://www.w3.org/1999/xhtml">\n')
f.write('<head>\n')
f.write('<meta http-equiv="content-type" content="text/html; charset=ISO-8859-1" />\n')
f.write('<link rel="stylesheet" type="text/css" href="style.css" />\n')
f.write('<title>%s</title>\n' % title)
f.write('</head><body>\n')
# for the benchmark
if len(benchmark) > 0:
  f.write('<h2>Colored Images of Manual Segmentation Benchmark in 19 Categories</h2>\n')
  f.write('<table style="text-align:left;height:116px;width:800px;margin-left:1em;" border="1" cellpadding="1" cellspacing="1">\n')
  f.write('<tbody>\n')
  count = 0
  for i in range(1, 21):
    if (count == 0):
      f.write('<tr>\n')
    count += 1
    if i <> 14:
      url = 'Benchmark/c%d.html' % i
      f.write('<td style="width:120px;">%d. <a href="%s">%s</a></td>\n' % (i,url,categories[i-1]))
    else:
      f.write('<td style="width:120px;">%d. %s</td>\n' % (i,categories[i-1]))
    if count == interval:
      count = 0
      f.write('</tr>\n')
  f.write('</tbody></table>\n')
# for algorithms side-by-side
if len(algorithms) > 0:
  f.write('<h2>Colored Images of %d Algorithms in 19 Categories Side-by-side</h2>\n' % len(algorithms))
  f.write('<table style="text-align:left;height:116px;width:800px;margin-left:1em;" border="1" cellpadding="1" cellspacing="1">\n')
  f.write('<tbody>\n')
  count = 0
  for i in range(1, 21):
    if (count == 0):
      f.write('<tr>\n')
    count += 1
    if i <> 14:
      url = 'sbs/c%d.html' % i
      f.write('<td style="width:120px;">%d. <a href="%s">%s</a></td>\n' % (i,url,categories[i-1]))
    else:
      f.write('<td style="width:120px;">%d. %s</td>\n' % (i,categories[i-1]))
    if count == interval:
      count = 0
      f.write('</tr>\n')
  f.write('</tbody></table>\n')
# for algorithms 
if len(algorithms) > 0:
  f.write('<h2>Colored Images of for Each Algorithms</h2>\n')
  f.write('<ul>\n')
  for a in algorithms:
    url = 'Algorithms/%s.html' % a.FullName
    f.write('<li><a href="%s">%s</a></li>\n' % (url,a.FullName))
  f.write('</ul>\n')
f.write('</body></html>\n')
f.close()


######################
# Benchmark/c*.html  #	
######################
if len(benchmark) > 0:
  dirOut = dirWWW + '%s/' % benchmark[0].FullName
  if not os.path.exists(dirOut):
    os.mkdir(dirOut)
  pathImg = '../../img/%s/' % benchmark[0].FullName # the relative img locations
  # read in the how many ground truth per model
  fn = dirPara + 'nSeg-BySegmentation.txt'
  nsegs = []
  f = open(fn, 'r')
  lines = f.readlines()
  f.close()
  for line in lines:
    nsegs.append(len(line.split()))
  # generate webpages 
  for i in range(1, 21): # html by category
    if i == 14: # skip "Spring"
      continue
    print "Generating c%d.html for Benchmark" % i
    fnHtml = dirOut + 'c' + str(i) + '.html'
    f = open(fnHtml, 'w')
    f.write('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">\n')
    title = 'Benchmark - Category ' + str(i) + ' - ' + categories[i-1]
    f.write('<html><head><title>%s</title></head>\n' % title)
    f.write('<link rel="stylesheet" type="text/css" href="../style.css" />\n')
    f.write('<body>\n')
    f.write('<h2>Manual Segmentation for 20 models in category %s</h2>\n' % categories[i-1])
    for j in range((i-1)*20+1, i*20+1):
      f.write('<h3>Model %d (%d segmentations)</h3>\n' % (j, nsegs[j-1]))
      f.write('<table style="text-align: left; width: 900px; height: 100px;" border="0" cellpadding="1" cellspacing="1"><tbody>\n')
      count = 0
      interval = 4
      dirImg = pathImg + str(j) + '/'
      for k in range(0, nsegs[j-1]):
        name = str(j) + '_' + str(k)
        fnImg = dirImg + name + '.bmp'
        if count == 0:
          f.write('<tr>\n')
        count = count + 1
        f.write('<td style="width: 240px;"><a href="%s"><img style="width: 256px; height: 192px;" border = "0" alt="%s" src="%s" /></a>\n' % (fnImg, name, fnImg))
        if(count == interval):
          count = 0
          f.write('</tr>\n')
      f.write('</tbody></table><br>\n')        
    f.write('</body></html>\n')
    f.close()

######################
# sbs/c*.html  	     #	
######################

# read in the nseg info
fn = dirPara + 'nSeg-%s.txt' % mode
f = open(fn,'r')
lines = f.readlines()
f.close()
nseg = range(0, 400)
for i in range(0, 400):
    nseg[i] = int(lines[i])

# set up directories and common contents
dirOut = dirWWW + 'sbs/'
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
pathImg = '../../img/'
nAlgorithms = len(algorithms)
content = 'Results from %d algorithms are listed below. ' % nAlgorithms
content += 'For the algorithms taking nSegment as input, nSegment is set according to %s mode. ' % mode
content += 'If an algorithm fails in one model, "NA" will appear instead of the colored image. '
content += 'You can click each image to view a larger image.'

# generate the html file
for c in range(1, 21):
  if c == 14: #skip the spring category
    continue
  print "Generating c%d.html for algoirthms side by side" % c
  fnHtml = dirOut + 'c' + str(c) + '.html' 
  f = open(fnHtml, 'w')
  f.write('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">\n')
  title = 'Colored Images - %d Algorithms Side by Side - Category %s' % (nAlgorithms,categories[c-1])    
  f.write('<html><head><title>%s</title></head>\n' % title)
  f.write('<link rel="stylesheet" type="text/css" href="../style.css" />\n')
  f.write('<body>\n')
  f.write('<h2>Results from %d algorithms side by side - Category %s</h2>\n' % (nAlgorithms,categories[c-1]))
  f.write('%s<br><br>\n' % content)
  f.write('<table style="text-align: center; width: 800px; height: 300px;" border="1" cellpadding="1" cellspacing="1"><tbody>\n')
  interval = 5
  count = 0
  fontsize = 2
  for i in range((c-1)*20+1, c*20+1): # rows
    if count == 0:
      f.write('<tr>\n')
      f.write('<td style="width:10px;text-align:center;background-color:rgb(0,0,0);color:rgb(255,255,255);"><font size="%d">%s</font></td>\n' % (fontsize,''))
      for a in algorithms:
        f.write('<td style="width:128px;text-align:center;background-color:rgb(0,0,0);color:rgb(255,255,255);"><font size="%d">%s</font></td>\n' % (fontsize,a.FullName))
      f.write('</tr>\n')
    count += 1
    if(count == interval):
      count = 0
    f.write('<tr><td style="width: 10px;text-align:center;">%d</td>\n' % i)
    for a in algorithms: # columns
      if a.AlgorithmType == 1:
        fnImg = pathImg + '%s/%d/%d_%d.bmp' % (a.FullName, i, i, nseg[i-1])
        fnSeg = dirSeg + '%s/%d/%d_%d.seg' % (a.FullName, i, i, nseg[i-1])
      else:
        fnImg = pathImg + '%s/%d.bmp' % (a.FullName, i)
        fnSeg = dirSeg + '%s/%d.seg' % (a.FullName, i)
      if not os.path.exists(fnSeg):
        f.write('<td style="width: 128px; text-align:center;">NA</td>\n')
        continue
      alt = a.FullName + str(i)
      f.write('<td style="width: 128px; text-align:center;" ><a href="%s"><img style="width: 128px; height: 96px; border: 0px" alt="%s" id="%s" src="%s"/></a>\n' % (fnImg, alt, alt, fnImg))
      f.write('</tr>\n')
  f.write('</tbody></table><br></body></html>\n')
  f.close()


######################
# Algorithms/c*.html #	
######################

# set up directories
dirOut = dirWWW + 'Algorithms/'
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
pathImg = '../../img/'
fontsize = 2
content1 = 'The algorithm requires numSegments as input.  We set it in %s mode.' % mode
content2 = 'The algorithm automatically predicts the number of segments.'

# generate webpages
for i in range(0, len(algorithms)):
  a = algorithms[i]
  print 'Generating a webpage for %s\'s segmentation results' % a.FullName
  fnHtml = dirOut + '%s.html' % a.FullName  
  f = open(fnHtml, 'w')
  f.write('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">\n')
  f.write('<html><head></head>\n')
  f.write('<link rel="stylesheet" type="text/css" href="../style.css" />\n')
  f.write('<body><title>Colored Images - %s</title>\n' % a.FullName)
  f.write('<h2>Results of %s</h2>\n' % a.FullName)
  if a.AlgorithmType == 1:
    content = content1
  else:
    content = content2
  f.write('%s<br><br>' % content)
  f.write('<table style="text-align: left; width: 900px; height: 300px;" border="1" cellpadding="1" cellspacing="1"><tbody>\n')
  count = 0
  interval = 7
  for i in fileIdRange:
    if a.AlgorithmType == 1:
      fnImg = pathImg + '%s/%d/%d_%d.bmp' % (a.FullName, i, i, nseg[i-1])
      fnSeg = dirSeg + '%s/%d/%d_%d.seg' % (a.FullName, i, i, nseg[i-1])
    else:
      fnImg = pathImg + '%s/%d.bmp' % (a.FullName, i)
      fnSeg = dirSeg + '%s/%d.seg' % (a.FullName, i)
    if not os.path.exists(fnSeg):
      continue
    if(count == 0):
      f.write('<tr>\n')
    count = count + 1
    f.write('<td style="width:128px; height:96px"><a href="%s"><img style="border: 0px; width: 128px; height: 96px;" alt="%d" src="%s"/></a></td>\n' % (fnImg, i, fnImg))
    if(count == interval):
      f.write('</tr>\n')
      count = 0
  f.write('</tbody></table><br></body></html>\n')
  f.close()


    

