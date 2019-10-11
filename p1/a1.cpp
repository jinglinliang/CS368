///////////////////////////////////////////////////////////////////////
// File Name:      a1.cpp
//
// Author:         Manyi Zhu
// CS email:       manyi@cs.wisc.edu
//
// Course:         CS 368 (C++)
// Section:        2
// Instructor:     Kuemmel
//
// Description:   This program reads two files of name list, and finds their intersection and union.
//
// Sources:
//
// URL(s) of sources:
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include<vector>
#include <string>
using namespace std;

int main(){

  //Load File1
  cout<<"Enter the name of the first file:";
  string filename1;
  getline(cin, filename1);
  ifstream infile1(filename1);
  while (!infile1.is_open()){
    cerr << "Input file "<<filename1<<" is NOT found. Please try again." << endl;
    cout<<"Enter the name of the first file:";
    getline(cin, filename1);
    infile1.open(filename1);
  }

  //Load File2
  cout<<"Enter the name of the second file:";
  string filename2;
  getline(cin, filename2);
  ifstream infile2(filename2);
  while (!infile2.is_open()) {
    cerr << "Input file "<<filename2<<" is NOT found. Please try again." << endl;
    cout<<"Enter the name of the second file:";
    getline(cin, filename2);
    infile2.open(filename2);
  }

  cout<<endl;


  string name;
  vector<string> v1; //For storing names in file1
  vector<string> v2; //For storing names in file2

    //Read file1 into v1
    int counter1 = 0;
    while (getline(infile1, name)){
        v1.push_back(name);
        counter1 = counter1 + 1;
    }

    cout<<"Number of students in "<<filename1<<" = "<<counter1<<endl;

    //Read file2 into v2
    int counter2 = 0;
    while (getline(infile2, name)){
         v2.push_back(name);
         counter2 = counter2 + 1;
     }

    cout<<"Number of students in "<<filename2<<" = "<<counter2<<endl;


    vector<string>::iterator it1;
    vector<string>::iterator it2;
    vector<string> v3(v1.size()+v2.size()); //For storing the intersection of names
    vector<string> v4; //For storing the union of names


    sort (v1.begin(), v1.end()); //sorting namelist1
    sort (v2.begin(), v2.end()); //sorting namelist2

    //Put intersection into in v3
    it1 = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
    v3.resize(it1-v3.begin());

    //Put union into in v4
    set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v4));

    cout<<"Number of students that are present in BOTH "<<filename1<<" AND "<<filename2<<" = "<<v3.size()<<endl;
    cout<<"Number of students that are present in EITHER "<<filename1<<" OR "<<filename2<<" = "<<v4.size()<<endl;

    cout<<endl;

    //Print list of union if exist
    if (v3.size() != 0) {
      cout<<"List of students that are present in BOTH "<<filename1<<" AND "<<filename2<<":"<<endl;
      for (it2=v3.begin(); it2!=v3.end(); it2++){
          cout<<*it2<<endl;
      }
      cout<<endl;
    }


    //Print to output files
    ofstream outFile1("intersection.txt");
    ofstream outFile2("union.txt");

    if (!outFile1.is_open() || !outFile2.is_open()){
      cerr<<"Unable to open the output file."<<endl;
    }
    else{

      if (v3.size() == 0){
        cout<<"The file intersection.txt is EMPTY since there are no students that are present in BOTH ";
        cout<<filename1<<" AND "<<filename2<<endl;
      }
      else{
        for (it2=v3.begin(); it2!=v3.end(); it2++){
            outFile1<<*it2<<endl;
        }
        cout<<"The names of the "<<v3.size()<<" students that are present in BOTH ";
        cout<<filename1<<" AND "<<filename2<<" are written to the file named intersection.txt"<<endl;
      }


      if (v4.size() == 0){
        cout<<"The file union.txt is EMPTY since there are no students in the 2 files ";
        cout<<filename1<<" AND "<<filename2<<endl;
      }
      else{
        for (it2=v4.begin(); it2!=v4.end(); it2++){
            outFile2<<*it2<<endl;
        }
        cout<<"The names of the "<<v4.size()<<" students that are present in EITHER ";
        cout<<filename1<<" OR "<<filename2<<" are written to the file named union.txt"<<endl;
      }
    }
}
