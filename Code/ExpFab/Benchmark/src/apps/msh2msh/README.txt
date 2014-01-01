This program converts a mesh file between different formats ("ply", "off" and "obj")

Usage: msh2msh fnMeshIn fnMeshOut [options]
======
fnMeshIn : the input mesh file
fnMeshOut : the output mesh file
[options]
  -v : output more information during running
  -flip : flip every face
  -scale scalefactor : scale the mesh by scalefactor
  -xform fnXform : transform the mesh according to the transform matrix in fnXform
  -max_edge_length max_length : split edges larger than max_length 

