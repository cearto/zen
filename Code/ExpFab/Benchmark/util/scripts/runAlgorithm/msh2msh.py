# import 
import os, sys
includepath = os.getcwd() + '/../../parameters/'
# a hack to produce includepath portable to both Linux and Windows
current = os.getcwd() 
os.chdir(includepath)
includepath = os.getcwd()
os.chdir(current)
sys.path.append(includepath)
from algorithmList import *

# get the file format as an input parameter
if len(sys.argv) <= 1:
  sys.exit()
format = sys.argv[1]
if (format <> "ply") and (format <> "obj"):
  print "Unsupported format! We only support PLY, OBJ and OFF files."
  sys.exit()

# generating meshes
print "Generating meshes in %s format" % format
root = os.getcwd()
dirIn = root + '/../../../data/off/'
dirOut = root + '/../../../data/%s/' % format
tool = root + '/../../../exe/msh2msh'
if not os.path.exists(dirOut):
  os.mkdir(dirOut)
for i in fileIdRange:
  fnIn = dirIn + str(i) + '.off'
  fnOut = dirOut + str(i) + '.' + format
  cmd = '%s %s %s' % (tool, fnIn, fnOut)
  os.system(cmd)
print "Done"

