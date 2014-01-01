/*
 * segAnalysis.C
 *
 * analysis of cut lengths, segment areas, dehedral angles, curvature, convexity
 *
 */
#include "R3Graphics/R3Graphics.h"
#include "AnalysisMethods.h"
using namespace std;


/*
 *  Global Variables
 */
int nFilesIn = 0;
char* fnMesh = NULL;
char* nameDirIn = NULL;
char* nameDirStat = NULL;
char* nameCurvRaw = NULL;
char* id = NULL;
char* namePathHull = NULL;
bool doArea = false;
bool doLength = false;
bool doAngle = false;
bool doCurv = false;
bool doConvexity = false;
bool doNumSegments = false;

/*
 * Argument Parsing
 */
static int
ParseArgs(int argc, char **argv){

	// Check number of arguments
	if (argc == 1) {
		printf("Usage:segAnalysis fnMesh -id id -nFilesIn N -dirStat nameDirStat -dirIn nameDirIn [options]\n");
		return 0;
	}

	// Parse arguments
	argc--; argv++;
	while (argc > 0) {
		if ((*argv)[0] == '-') {
			if (!strcmp(*argv, "-v")) print_verbose = 1;
			else if (!strcmp(*argv, "-area")) {doArea = true;}
         		else if (!strcmp(*argv, "-numSegments")) {doNumSegments = true;}
			else if (!strcmp(*argv, "-length")) {doLength = true;}
			else if (!strcmp(*argv, "-angle")) {doAngle = true;}	
			else if (!strcmp(*argv, "-curv")) {doCurv = true;argv++;argc--;nameCurvRaw=*argv;}
			else if (!strcmp(*argv, "-dirIn")) {argv++; argc--; nameDirIn = *argv;}
			else if (!strcmp(*argv, "-dirStat")) {argv++; argc--; nameDirStat = *argv;}
			else if (!strcmp(*argv, "-id")) {argv++; argc--; id = *argv;}
			else if (!strcmp(*argv, "-convexity")) {doConvexity = true; argv++; argc--; namePathHull = *argv;}
			else if (!strcmp(*argv, "-nFilesIn")) {argv++; argc--; nFilesIn = atoi(*argv);}
			else { fprintf(stderr, "Invalid program argument: %s", *argv); return 0; }
			argv++; argc--;
		}
		else {
			if (!fnMesh) fnMesh = *argv;
			else {fprintf(stderr, "Invalid program argument: %s\n", *argv); return 0;}
			argv++; argc--;
		}
	}
	
	// check file names
	if (nFilesIn < 1) {cerr << "Not enough files to analyze!\n"; return 0;}
	if(doCurv && !nameCurvRaw) {cerr << "Please give curvature info!\n"; return 0;}
	if(doConvexity && !namePathHull) {cerr << "Please give convexity hull info!\n"; return 0;}
	
	// Return OK status
	return 1;
}


/*
 * Main
 */
int main(int argc, char **argv){

	// Start statistics
	RNTime start_time;
	start_time.Read();
	
	// Check number of arguments
	if (!ParseArgs(argc, argv)) exit(1);
	
	// read in all segmentations
	R3Mesh** meshesIn = new R3Mesh*[nFilesIn];
	Segmentation** segmentationsIn = new Segmentation*[nFilesIn];
	for(int i = 0; i < nFilesIn; i ++) {
		string name(id);
		std::stringstream out; out << i;
		name += "_"; name+= out.str();
		cout << name << endl;
		string str = MakeName(nameDirIn, name.c_str(),(char*)".seg");
		meshesIn[i] = ReadMesh(fnMesh, str.c_str());
		segmentationsIn[i] = CreateSegmentation(meshesIn[i]);
	}
	if (print_verbose) 
		printf("Spend %.2f seconds to read in all %2d segmentations\n", start_time.Elapsed(), nFilesIn);
	
	// verbose...
	if(print_verbose) {
		cout << "There are " << nFilesIn << " input files from directory " << nameDirIn << ":\n";
		cout << "Stat files will be put in " << nameDirStat << endl;
		string statItems;
		if(doArea) statItems.append("area ");
		if(doAngle) statItems.append("angle ");
		if(doCurv) statItems.append("curvature ");
		if(doConvexity) statItems.append("convexity ");
		if(doLength) statItems.append("length ");
            	if(doNumSegments) statItems.append("numSegments  ");      
		cout << "Stat items: " << statItems <<  endl;
	}
	
	// analyze and output results
	if(doArea) analyzeSegmentArea(segmentationsIn, nFilesIn, nameDirStat, id, print_verbose);
	if(doAngle) analyzeDihedralAngle(segmentationsIn, nFilesIn, nameDirStat, id, print_verbose);
	if(doCurv) analyzeCurvature(segmentationsIn, nFilesIn, nameDirStat, id, nameCurvRaw, print_verbose);	
	if(doConvexity) analyzeConvexity(segmentationsIn, nFilesIn, nameDirStat, id, namePathHull, print_verbose);
	if(doLength) analyzeCutLength(segmentationsIn, nFilesIn, nameDirStat, id, print_verbose);
	if(doNumSegments) analyzeNumSegments(segmentationsIn, nFilesIn, nameDirStat, id, print_verbose);
	
	// clean
	for(int i = 0; i < nFilesIn; i ++) { delete meshesIn[i]; delete segmentationsIn[i]; }
	delete meshesIn;
	delete segmentationsIn;
	
	// verbose again...
	if (1) {
		cout << "*********************************************************************\n";
		cout << "Statistics for model " << id << " is acquired!" << endl;
		cout << "Total time = " << start_time.Elapsed() << endl;
	}
	
	// return success
	return 1;
}

