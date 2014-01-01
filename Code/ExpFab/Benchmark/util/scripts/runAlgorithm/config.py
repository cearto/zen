################################################################## 
# This is the configure files for running your batch executable ##
# segmentation program, please set the following parameters	##
################################################################## 

# name of your algorithm
name = 'SuperSeg' # change it to what you call your program
# whether your algorithm takes in the number of segments as input
takeNSegment = False # True
# if takeNSegment = True, need to tell us how to set nSegments
optionNSegment = '-nsegment' # change to what you use in your program
# where is your batch executable
tool = '../../../exe/SuperSeg' # (please use relative path, and no backslash '\' within a path)
# other options to run your programs, e.g, some fixed parameters
options = '-v'
# You may also want to set the mesh format that your algorithm could handle
# in "../../parameters/algorithmList.py"
# we assume "off" as the default while also supporting "ply" and "obj"


# We assume that your program runs in the following format, otherwise,
# you may need to write your own script to run your programs on our
# data set
# $(tool) $(fnMeshIn) $(fnMeshOut) $(fnSegOut) [-nsegment $(numSegment)] [options] 
# $(tool): 	your program
# $(fnMeshIn): 	the input mesh
# $(fnMeshOut): the output mesh (some programs will change mesh geometries)
# $(fnSegOut): 	the segmentation results, line[i] corresponds to the segment ID of face[i]
# $(numSegment):number of segments  

# Your final segmentations will be put in
# dir = '../../../data/seg/$(name)/'
# if takeNSegment = 'True', the segmentation for model 180 and numSegment = 10 is in
# '../../../data/seg/$(name)/180/180_10.seg'
# else, the segmentation for model 180 is in
# '../../../data/seg/$(name)/180.seg' 

# After generating the necessary segmentations, you may add your algorithm $(name)
# to the algorithm list in '../../parameters/algorithmList.py'


