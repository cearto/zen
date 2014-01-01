/*
 * GroupSegEval.C
 *
 * Pair-wise evaluation of segmentations of the same model
 *
 */

#include "R3Shapes/R3Shapes.h"
#include "Segmentations.h"
#include "Entry.h"
#include "EvaluationMethods.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

char* fnMesh = NULL;
char* dirIn = NULL;
char* dirOut = NULL;
char* id = NULL;
int N = 0; // the number of input segmentations
bool doCE = false;
bool doCD = false;
bool doHD = false;
bool doRI = false;
bool doAll = true;


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
 * Group evaluation
 */
void GroupSegEval(char* fnMesh, char* dirIn, char* dirOut, int N) {
	
	// parameters to use
	R3Mesh **meshes = new R3Mesh*[N];
	Segmentation **segmentations = new Segmentation*[N];
	vector<string> fnIns;
	Entry **entries = new Entry*[N*N];
	for(int i = 0; i < N*N; i ++) entries[i] = NULL;
	
	// read in all the meshes
	// Start statistics
	RNTime start_time;
	start_time.Read();
	if(print_verbose) {
		cout << "Reading in " << N << " meshs from " << dirIn << "...\n";
	}
	for(int i = 0; i < N; i ++) {
		string name, tmp;
		stringstream ss; ss << i; tmp = ss.str();
		name.assign(id); name = name + "_" + tmp;
		fnIns.push_back(MakeName(dirIn, name.c_str(), (char*)(".seg"))); 
		meshes[i] = ReadMesh(fnMesh, fnIns[i].c_str());
		if (!meshes[i]) {cerr << "Cannot read mesh " << fnIns[i] << endl; exit(1); }
		segmentations[i] = CreateSegmentation(meshes[i]);
		if (!segmentations[i]) {cerr << "Cannot create segmentation for " << fnIns[i] << endl; exit(1);}
	}
	if(print_verbose) {
		printf("All meshes read in!\n");
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		fflush(stdout);
	}
	
	// evaluating
	if(print_verbose) { cout << "Group evaluation begins..." << endl;}
	for(int i = 0; i < N; i ++) {
		for(int j = i+1; j < N; j ++) {
			// create entries
			entries[i*N+j] = new Entry;
			if(doCE) {
				if(print_verbose) {printf("comparing %d with %d in CE...\n", i, j);}
				entries[i*N+j]->e_CE = EvaluateConsistencyError(segmentations[i], segmentations[j]);
			}
			if(doHD) {
				if(print_verbose) {printf("comparing %d with %d in HD...\n", i, j);}
				entries[i*N+j]->e_HD = EvaluateHammingDistance(segmentations[i], segmentations[j]);
			}
			if(doRI) {
				if(print_verbose) {printf("comparing %d with %d in RI...\n", i, j);}
				entries[i*N+j]->e_RI = EvaluateRandIndex(segmentations[i], segmentations[j]);
			}
			if(doCD) {
				if(print_verbose) {printf("comparing %d with %d in CD...\n", i, j);}
				entries[i*N+j]->e_CD = EvaluateCutDiscrepancy(segmentations[i], segmentations[j]);
			}
		}
	}
	if(print_verbose) {
		printf("All evaluations finished!\n");
		printf("  Time = %.2f seconds\n", start_time.Elapsed());
		fflush(stdout);
	}
	
	// output to a file
	string name;
	ofstream fout;
	// dump CE
	if(doCE){
		name = MakeName(dirOut, id, (char*)(".CE"));
		fout.open(name.c_str(), ios::out);
		if(!fout.is_open()) {cerr << "Cannot dump the CE results" << endl;}
		for(int i = 0; i < N; i ++) {
			for(int j = i + 1; j < N; j ++) {
				Entry* e = entries[i*N+j];
				if(!e) continue;
				if(e->e_CE) {
					fout << e->e_CE->GCE << "\t" << e->e_CE->LCE << "\t" << e->e_CE->GCEa << "\t" << e->e_CE->LCEa << "\n";
				}
			}
		}
		fout.close();
		if(print_verbose) cout << "CE results dumped!" << endl;
	}
	// dump HD
	if(doHD){
		name = MakeName(dirOut, id, (char*)(".HD"));
		fout.open(name.c_str(), ios::out);
		if(!fout.is_open()) {cerr << "Cannot dump the HD results" << endl;}
		for(int i = 0; i < N; i ++) {
			for(int j = i + 1; j < N; j ++) {
				Entry* e = entries[i*N+j];
				if(!e) continue;
				if(e->e_HD) {
					// one evaluation, two results
					fout << e->e_HD->distance << "\t" << e->e_HD->missingRate << "\t" << e->e_HD->falseAlarmRate << "\n";
					fout << e->e_HD->distance << "\t" << e->e_HD->falseAlarmRate << "\t" << e->e_HD->missingRate << "\n";
				}
			}
		}
		fout.close();
		if(print_verbose) cout << "HD results dumped" << endl;
	}
	// dump RI
	if(doRI){
		name = MakeName(dirOut, id, (char*)(".RI"));
		fout.open(name.c_str(), ios::out);
		if(!fout.is_open()) {cerr << "Cannot dump the RI results" << endl;}
		for(int i = 0; i < N; i ++) {
			for(int j = i+1; j < N; j ++) {
				Entry* e = entries[i*N+j];
				if(!e) continue;
				if(e->e_RI) {
					fout << e->e_RI->RI << "\n";
				}
			}
		}
		fout.close();
		if(print_verbose) cout << "RI results dumped" << endl;
	}	
	// dump CD
	if(doCD){
		name = MakeName(dirOut, id, (char*)(".CD"));
		fout.open(name.c_str(), ios::out);
		if(!fout.is_open()) {cerr << "Cannot dump the CD results" << endl;}
		for(int i = 0; i < N; i ++) {
			for(int j = i+1; j < N; j ++) {
				Entry* e = entries[i*N+j];
			
				if(!e) continue;
				if(e->e_CD) {
					fout << e->e_CD->CD << "\n";
				}
			}
		}
		fout.close();
		if(print_verbose) cout << "CD results dumped" << endl;
	}

	// clean
	for(int i = 0; i < N; i ++) {
		if(segmentations[i]) { delete segmentations[i]; }
		if(meshes[i]) { delete meshes[i]; };
	}
	for(int i = 0 ; i < N*N; i ++){
		if(entries[i]) { delete entries[i]; }
	}
	delete segmentations; 
	delete meshes;
	delete entries; 
}


/*
 * Argument Parsing
 */
static int
ParseArgs(int argc, char **argv){

	// Check number of arguments
	if (argc == 1) {
		printf("Usage: groupSegEval fnMesh dirIn dirOut modelId nSegs [options]\n");
		exit(0);
	}

	// Parse arguments
	argc--; argv++;
	while (argc > 0) {
		if ((*argv)[0] == '-') {
			if (!strcmp(*argv, "-v")) print_verbose = 1;
			else if(!strcmp(*argv, "-all")) {doAll = true;}
			else if(!strcmp(*argv, "-CD")) {doCD = true; doAll = false;}
			else if(!strcmp(*argv, "-CE")) {doCE = true; doAll = false;}
			else if(!strcmp(*argv, "-HD")) {doHD = true; doAll = false;}
			else if(!strcmp(*argv, "-RI")) {doRI = true; doAll = false;}
			else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
			argv++; argc--;
		}
		else {
			if (!fnMesh) fnMesh = *argv;
			else if (!dirIn) dirIn = *argv;
			else if (!dirOut) dirOut = *argv;
			else if (id == NULL) id = (*argv);
			else if (N == 0) N = atoi(*argv);
			else { fprintf(stderr, "Invalid program argument: %s", *argv); exit(1); }
			argv++; argc--;
		}
	}
	
	// check directory names
	if (!fnMesh || !dirIn || !dirOut) {
		fprintf(stderr, "Please specify the input, ref, and stat files!\n");
		return 0;
	}
	
	// if there is enough files
	if (N < 2) {cerr << "No enough files to evaluate!\n"; exit(5);}
	
	if(doAll) {
		doCD = doCE = doHD = doRI = true;
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

	// group evaluation
	GroupSegEval(fnMesh, dirIn, dirOut, N);
	
	// Return success
	return 0;
}

