###################################################
# Global Flags					  #
###################################################

# whether you need to run a new algorithm
# if True, please make sure you correctly configured the script in 
# "../scripts/runAlgorithm/config.py"
doRunAlgorithm = False 
#doRunAlgorithm = True  # reset to True 
  
# whether you want to run evaluations
doEvaluation = False
doEvaluation = True  # reset to True

# wheter you want to run analyses
doAnalysis = False
#doAnalysis = True # reset to True

# whether to visualize your evaluation and/or analysis results
# if True, please make sure that Matlab is installed
doVisualization = False 
doVisualization = True # reset to True


###################################################
# The 3D Mesh File Format you want to work with   #
# Default: .off                                   #
###################################################
Mesh_Format = 'off' # 'obj', 'ply'


###################################################
# Edit the following arrays to include or exclude #
# an algorithm                                    #
###################################################

class AlgorithmClass:
  def __init__(self, FullName, SplittedName1, SplittedName2, AlgorithmType, needEvaluation, needAnalysis, needColoredImage):
    # the full name of your algorithm
    self.FullName = FullName
    # a two-part alias name to represent the algorithm (for visualization)
    self.SplittedName1 = SplittedName1
    self.SplittedName2 = SplittedName2
    # AlgorithmType 
    # 1: the algorithm takes numSegment as input
    # 2: the algorithm automatically decides numSegment
    # 3: it is the human benchmark 
    self.AlgorithmType = AlgorithmType
    # The following three flags decides whether to evaluate/analyze/make colored images for segmentations 
    # of the current algorithm. Once you have generated these results, you may prefer to set them to False so that you may
    # not need to repeat the efforts
    self.needEvaluation = needEvaluation
    self.needAnalysis = needAnalysis
    self.needColoredImage = needColoredImage  

# please list the algorithms in the order that you want to finally visualize them
# you can include or exclude an algorithm by adding or commenting one line with "#"
allAlgorithms = [
  AlgorithmClass('Benchmark', 'Human', '',      3,  True,  True,  True),
  #AlgorithmClass('RandCuts',  'Rand',  'Cuts',  1,  True,  True,  True),
  #AlgorithmClass('ShapeDiam', 'Shape', 'Diam',  2,  True,  True,  True),
  #AlgorithmClass('NormCuts',  'Norm',  'Cuts',  1,  True,  True,  True),
  #AlgorithmClass('CoreExtra', 'Core',  'Extra', 2,  True,  True,  True),
  #AlgorithmClass('RandWalks', 'Rand',  'Walks', 1,  True,  True,  True),
  #AlgorithmClass('FitPrim',   'Fit',   'Prim',  1,  True,  True,  True),
  #AlgorithmClass('KMeans',    'K-',    'Means', 1,  True,  True,  True)
]


###################################################
# Range of models to be included in the study	  #
###################################################
fileIdRange = range(1, 261) + range(281, 401)


###################################################
# Do not edit the following stuff                 #
###################################################

# 20 categories in the dataset, "Spring" is not included in our study
categories = [
'Human', 'Cup', 'Glasses', 'Airplane', 'Ant',
'Chair', 'Octopus', 'Table', 'Teddy', 'Hand',
'Plier', 'Fish', 'Bird', 'Spring', 'Armadillo',
'Bust', 'Mech', 'Bearing', 'Vase', 'FourLeg'
]

