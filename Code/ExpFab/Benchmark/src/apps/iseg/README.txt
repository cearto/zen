This program is an interactive tool for segmenting 3D meshes. 

Usage: iseg fnMesh fnSeg [options]
======
fnMesh : the path to the input mesh model
fnSeg : the path to the output segmentation (".seg" file)
[options]
  -v : output more information during running
  -debug : output debug information during running
  -image fnImage : save the colored image (only in ".bmp") of segmentation to fnImage
 
The program provides several commands (e.g., 'a') for creating and
editing a path through vertices of the mesh and a command ('c') for
cutting the mesh into two segments by separating along the current
path.  Each "cut" partitions a single segment into two, and thus these
simple commands can be applied in sequence to decompose a mesh into
multiple segments.  The segmentation result will be saved to the
output_ply_file when the program exits (hit the ESCAPE key to quit
the program).

The following commands provide ways to create/edit cut paths:
  'a' or Insert: Insert anchor point into path at vertex closest to cursor location
  'd' or Backspace or Delete: Remove anchor point at vertex closest to cursor location
  'c' or Enter: Cut one segment into two along the current cut path
  's': Save the current segmentation to the output file 
  'r': Reset/empty the current cut path (remove all anchor points)
  'u': Undo the most recent cut
  ESC - save and exit
  UPARROW - start/goto next cut
  DOWNARROW - goto previous cut

The anchor points are shown as yellow spheres and the cut path is shown as
a yellow line on the mesh.  You will need to distribute anchor points at
several places along the intended seam of the mesh to define cut paths 
precisely (the path between anchor points follows the shortest path on
the mesh surface) before making cuts.

There are several commands for changing the way the current segmentation is displayed:
  'A': Toggle display of anchor points (yellow spheres)
  'E': Toggle display of mesh edges (thin red lines)
  'F': Toggle display of mesh faces (colored polygons)
  'V': Toggle display of mesh vertices (small blue spheres)
  'P': Toggle display of current cut path (yellow line along mesh)
  'B': Toggle display of backfacing polygons 
  'S': Toggle coloring of faces by segment (otherwise all faces are gray)
  ' ': Center future viewpoint rotations and scales at surface position under cursor

The last command (that centers future viewpoint rotations and scales) is very
helpful for fine-scale control of the camera when making cuts in localized regions.

To navigate the scene:
  Keep left mouse button down and move the mouse to rotate the scene
  Keep middle mouse button down and move the mouse to scale the scene
  Keep right mouse button down and move the mouse to translate the scene

Read function "GLUTKeyboard", "GLUTSpecial" and "ParseArgs" for more details.

Please feel free to contact funk@cs.princeton.edu with questions or comments.


