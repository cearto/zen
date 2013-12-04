To quickly run your experiment, you could use "./runAll.py"., Please read it
and do the necessary configurations before running it.  

"runAll.py" aggregates all scripts in sub-directories of the current directory as
below:

This directory contains scripts to help you:

1. run your algorithm and plug in your segmentations for study
   see "./runAlgorithm/README.txt"

2. evaluate segmentations from algorithms against the human benchmark
   see "./evaluate/README.txt"

3. analyze properties of segmentations from each algorithm
   see "./analyze/README.txt"

4. visualize evaluation and analysis results using Matlab 
   see "./visualize/README.txt"

Evaluation and analysis will take a while, but results for the given
algorithms and benchmark only need to be generated once. 

If you have a multi-core machine or cluster, you may explictly parallelize the
running process by editing "../parameters/algorithmList.py" to process one
algorithm each time on one core, thus running all algorithms at the same time. 



