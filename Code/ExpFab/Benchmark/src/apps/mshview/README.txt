This program is for viewing a mesh model.

Usage: mshview fnMesh [-seg fnSeg] [options]
======
fnMesh : path to the mesh model
-seg fnSeg : path to the segmentation file
[options]
  -v : output more information during running 
  -image fnImage : output the colored image (only in ".bmp") of the model and exit the program
  -vertices : to show vertices
  -edges : to show edges
  -back : to show backfaces 
  -axes : to show axes
  -segments : to show segments in different colors
  -camera 9CameraParameters : to initialize the camera with 9 camera parameters

To navigate the scene:
  Keep left mouse button down and move the mouse to rotate the scene
  Keep middle mouse button down and move the mouse to scale the scene
  Keep right mouse button down and move the mouse to translate the scene

Read function "ParseArgs" and "GLUTKeyboard" for more usages.
   

