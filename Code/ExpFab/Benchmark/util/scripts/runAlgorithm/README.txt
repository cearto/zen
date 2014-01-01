Scripts here are to help you run your algorithm (a batch executable) on
our mesh data set to generate required segmentations for comparisons with
other algorithms.

Please follow the following procedure:

1. Skip if your program could handle meshes in "OFF" format. 
Otherwise, please use the command below to generate meshes in other formats. 
(for now, we only support "ply", "obj" and "off" files)
run "python msh2msh.py ply" to generate "ply" mesh files of our data set
run "python msh2msh.py obj" to generate "obj" mesh files of our data set
Results will be in "../../../data/ply/" or "../../../data/obj/"

2. Edit "config.py" to configure how to use your segmentation program

3. Run "python runAlgorithm.py" to get the initial segmentations.
It will also pre-process your initial segmentations for the following purposes:
(1) make sure that each segment is a connected part
(2) if your algorithm changes mesh geometry (e.g, split edges), map your
results back to our original mesh geometry
(3) output ".seg" files which are compatible with our provided meshes (segment
ID sequence matches face sequences in our mesh model) 
(4) re-arrange segment IDs from 0 to (numSegment-1)

