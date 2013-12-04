This program analyzes the following properties:
1. Segment area
2. Cut length
3. Dihedral angle
4. Curvature (mean, max, min, gaussian)
5. Convexity and Compactness(relying on the inputs from Qhull results)
6. NumSegments

Usage: segAnalysis fnMesh -id id -nFilesIn N -dirStat nameDirStat -dirIn nameDirIn [options]
======
fnMesh : path to the mesh model
-id id : id of the mesh model
-nFilesIn N : number of segmentations for the model
-dirStat nameDirStat : directory to store the analysis results
-dirIn nameDirIn : directory of input segmentations
[options]
  -v : output more information during running
  -area : to analyze area related properties
  -numSegments : to analyze number of segments
  -length : to analyze cut boundary length related properties 
  -angle : to analyze dihedral angle related properties
  -curv fnCurv : to analyze curvature related properties (need curvature info from fnCurv)
  -convexity fnHull : to analyze convexity and compactness (need convex hull info from fnHull)
   
Read function "ParseArgs" for more details.

