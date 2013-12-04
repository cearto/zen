
Please be aware that Matlab is required to generate plots with our scripts.


VISUALIZE EVALUATION RESULTS
============================

1. Generate plots for evaluation scores
Run "python plotEvaluationResults.py"
It will also create a webpage for all plots in
"../../../evaluation/report/report.html"

2. Create a pdf report with all plots and tables (only in Linux)  
Run "python createEvaluationReport.py"
The pdf file will be in "../../../evaluation/report/report.pdf"


VISUALIZE ANALYSIS RESULTS
==========================

1. Generate plots for property statistics
Run "python plotAnalysisResults.py"
It will also create a webpage for all plots in
"../../../analysis/report/report.html"

2. Create a pdf report for all plots (only in Linux)
Run "python createAnalysisReport.py"
The pdf file will be in "../../../analysis/report/report.pdf"


OTHERS
======

1. Generate colored images in "bmp" format for mesh segmentations
Run "python genImg.py"
The results will be in "../../../data/img/"

2. Generate webpages to view colored images
Run "python genHtml.py"
The results will be in "../../../data/www/index.html"

