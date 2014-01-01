/*
 * Entry.h
 * 
 * Define the structs to book evaluations results
 * 
 */

#ifndef ENTRY_H_
#define ENTRY_H_


/*
 * The entry to book Cut Discrepancy results
 */
struct Entry_CD{
	double CD;
};

/*
 * The entry to book Consistency Error results
 */
struct Entry_CE{
	double GCE;
	double LCE;
	double GCEa;
	double LCEa;
};

/*
 * The entry to book Hamming Distance
 */
struct Entry_HD{
	double distance;
	double missingRate;
	double falseAlarmRate;
};

/*
 * The entry to book Rand Index related measure
 */
struct Entry_RI{
	double RI;			// Rand Index
};


/* 
 * The entry to book all evaluations
 */
struct Entry{
	char name[50];
	Entry_CE *e_CE;	// Consistency Error
	Entry_HD *e_HD;	// Hamming Distance
	Entry_CD *e_CD;	// Cut Discrepancy
	Entry_RI *e_RI;	// Rand Index
	~Entry();
};
Entry::~Entry() {
	delete e_CE;
	delete e_HD;
	delete e_CD;
	delete e_RI;
}

#endif /*ENTRY_H_*/

