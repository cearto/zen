/*
 * SegEval.C
 *
 * Quantitative comparison between two mesh segmentations
 *
 */


/*
 * Include files
 */
#include "R3Shapes/R3Shapes.h"
#include "Segmentations.h"
#include "Entry.h"
#include "EvaluationMethods.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


/*
 * Program arguments
 */
static char *fnModel = NULL;
static char *fnIn = NULL;
static char *fnRef = NULL;
static char *dirStat = NULL;
extern int print_verbose;
extern int print_debug;
bool EnableAll = true;		// enable all measures, default
bool EnableCE = false; 		// enable consistency error
bool EnableHD = false;		// enable Hamming distance
bool EnableCD = false;		// enable cut discrepancy
bool EnableRI = false;		// enable Rand Index 




/**************************************************************************************************
 **************************************************************************************************
 * Global Evaluator : Call all evaluation methods *************************************************
 **************************************************************************************************
 **************************************************************************************************/

/*
 * Make a filename by combine dir, name and suffix
 */
string MakeName(char* dir, const char* name, char* suffix) {
	string fn = dir;
	if(fn.at(fn.length()-1) != '/') fn += "/";
	fn += name;
	fn += suffix;
	return fn;
}

/*
 * Evaluate one segmentation(fnIn) by comparing it with ground truth(fnRef)
 * Output the statistics(fnStat)
 */
int EvaluateOne(char* fnModel, char* fnIn, char* fnRef, char* dirStat){

	// Start statistics
	RNTime start_time;
	start_time.Read();
	
	// Get the model name withouth path and suffix
	string str = fnRef;
	int p = str.find_last_of('/');
	str = str.substr(p+1);
	p = str.find_first_of('.');
	str = str.substr(0, p);
	const char *name = str.c_str();
	
	// Read input mesh
	R3Mesh *mesh = ReadMesh(fnModel, fnIn);
	if (!mesh) {
		cerr << "Cannot create segmented mesh from " << fnModel << " and " << fnIn << endl; 
		exit(-1);
	}

	// Create segmentation
	Segmentation *s1 = CreateSegmentation(mesh);
	if (!s1) exit(-1);

	// Read reference mesh
	R3Mesh *reference_mesh = ReadMesh(fnModel, fnRef);
	if (!reference_mesh) exit(-1);

	// Create segmentation
	Segmentation *s2 = CreateSegmentation(reference_mesh);
	if (!s2) exit(-1);
	
	// create the booking entry
	Entry *e = new Entry;
	
	// Call evaluation methods
	ofstream fStat;
	string fn;
	strcpy(e->name, name);
	if(EnableCE){ // Consistency Error
		e->e_CE = EvaluateConsistencyError(s1, s2);
		if(e->e_CE) {
			fn = MakeName(dirStat, name, (char*)(".CE"));
			fStat.open(fn.c_str(), ios::out);
			if(!fStat) {cerr << "Cannot create Consistency Error statistics for " << e->name << endl; exit(1);}
			fStat << e->e_CE->GCE << "\t" << e->e_CE->LCE << "\t" << e->e_CE->GCEa << "\t" << e->e_CE->LCEa << "\n";
			fStat.close();
		}
	}
	if(EnableHD){ // Hamming Distance
		e->e_HD = EvaluateHammingDistance(s1, s2);
		if(e->e_HD) {
			fn = MakeName(dirStat, name, (char*)(".HD"));
			fStat.open(fn.c_str(), ios::out);
			if(!fStat) {cerr << "Cannot create Hamming Distance statistics for " << e->name << endl; exit(1);}
			fStat << e->e_HD->distance << "\t" << e->e_HD->missingRate << "\t" << e->e_HD->falseAlarmRate << "\n";
			fStat.close();
		}
	}	
	if(EnableCD){ // Cut Discrepancy
		e->e_CD = EvaluateCutDiscrepancy(s1, s2);
		if (e->e_CD) {
			fn = MakeName(dirStat, name, (char*)(".CD"));
			fStat.open(fn.c_str(), ios::out);
			if(!fStat) {cerr << "Cannot create Cut Discrepancy statistics for " << e->name << endl; exit(1);}
			fStat << e->e_CD->CD << "\n";
			fStat.close();
		}
	}
	if(EnableRI){ // Rand Index
		e->e_RI = EvaluateRandIndex(s1, s2);
		if (e->e_RI){
			fn = MakeName(dirStat, name, (char*)(".RI"));
			fStat.open(fn.c_str(), ios::out);
			if(!fStat) {cerr << "Cannot create Rand Index statistics for " << e->name << endl; exit(1);}
			fStat << e->e_RI->RI << "\n";
			fStat.close();
		}
	}
	
	// Output
	cout << "*********************************************************************\n";
	cout << "Evaluation for " << e->name << " finished!" << endl;
	cout << "Total time = " << start_time.Elapsed() << endl;

	// clean up
	delete mesh;
	delete reference_mesh;
	delete s1;
	delete s2;	
	delete e;

	// return success
	return 1;
}



/*
 * Argument Parsing
 */
static int
ParseArgs(int argc, char **argv){

	// Check number of arguments
	if (argc < 5) {
		printf("Usage: segEval fnMesh fnSeg fnSegRef dirStat [options]\n");
		exit(0);
	}

	// Parse arguments
	argc--; argv++;
	while (argc > 0) {
		if ((*argv)[0] == '-') {
			if (!strcmp(*argv, "-v")) print_verbose = 1;
			else if (!strcmp(*argv, "-debug")) print_debug = 1;
			else if (!strcmp(*argv, "-All")) {EnableAll = true;}
			else if (!strcmp(*argv, "-CE")) {EnableCE = true; EnableAll = false;}
			else if (!strcmp(*argv, "-HD")) {EnableHD = true; EnableAll = false;}
			else if (!strcmp(*argv, "-CD")) {EnableCD = true; EnableAll = false;}
			else if (!strcmp(*argv, "-RI")) {EnableRI = true; EnableAll = false;}
			else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
			argv++; argc--;
		}
		else {
			if (!fnModel) fnModel = *argv;
			else if(!fnIn) fnIn = *argv;
			else if (!fnRef) fnRef = *argv;
			else if (!dirStat) dirStat = *argv;
			else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
			argv++; argc--;
		}
	}
	
	// Set flags
	if(EnableAll){
		EnableCE = EnableHD = EnableCD = EnableRI = true;
	}

	// Check input filename
	if (!fnModel || !fnIn || !fnRef) {
		fprintf(stderr, "Please specify the model, input, ref, and stat files!\n");
		return 0;
	}

	// Return OK status
	return 1;
}


/*
 * Main
 */
int main(int argc, char **argv){

	// Check number of arguments
	if (!ParseArgs(argc, argv)) exit(1);

	// Evaluate one 3D model
	EvaluateOne(fnModel, fnIn, fnRef, dirStat);
	
	// Return success
	return 0;
}

