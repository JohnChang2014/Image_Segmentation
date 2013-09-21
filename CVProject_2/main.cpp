/*
 *  CS 6643   Fall  2012
 *  Project 2:  Image Segmentation
 *  Programmer: Ching-Che Chang
 *  Description:
 *      This program is designed for image segmentation in the color space with iterating K-mean algorithm
 *      and built on the environment of Mac OS X Mountain Lion with the tool of Xcode. Since Mac OS X is an 
 *      Unix-like OS, the program is compatible with the OS like Unix and Linux if using the same compilers. 
 *      This program is compiled by GNU compiler collection (GCC) of the latest version 4.7.2, which is suitable 
 *      for operating systems, such as Unix, Linux, and Mac OS X. Once you install the compiler, you will be 
 *      able to compile and run this program on your computer with those operating systems listed above.
 *
 */

#include "config.h"
#include "FileController.cpp"
#include "Cluster.cpp"

int main() {
    vector<string> inputdata;
    FileController myFileController;
    Cluster myCluster;
    
    inputdata   = myFileController.requestInputs();
    int start_k = 2;
    int end_k   = 2;
    
    
    // read input data
    int n_params       = (int) inputdata.size();
    string currentPath = myFileController.getCurrentPath();
    string filePath    = inputdata.at(0);
    int height         = atoi(inputdata.at(1).c_str());
    int width          = atoi(inputdata.at(2).c_str());
        
    // set up K values
    if (n_params == 4) {
        start_k = atoi(inputdata.at(3).c_str());
        end_k   = start_k;
    } else if (n_params == 5) {
        start_k = atoi(inputdata.at(3).c_str());
        end_k   = atoi(inputdata.at(4).c_str());
    }
        
    // read input data
    vector<vector<pixel> > imageVec = myFileController.readFile(filePath, height, width);
    vector<vector<pixel> > imgVec   = imageVec;
    vector<vector<int> > d_value_set;
    cout << currentPath << endl;
    
    // perform iterating k-mean algorithm from start_k to end_k
    for (int k = start_k; k <= end_k; k++) {
        string inputfileName =  currentPath + "/" + myFileController.getFileNameFromPath(filePath, true) + "_k=" + myFileController.toString(k) + ".raw";
        // start to run algorithm with a specific k
        myCluster.runKMeanAlgorithm(imgVec, k);
            
        // record D value against this k value
        d_value_set.push_back(myCluster.getDValues());
            
        // produce a raw image as the result for this value
        myFileController.outputRAW(imgVec, inputfileName);
        
        // reset imgVec array for another computation
        imgVec = imageVec;
    }
    
    // output final D values against all K values
    string outputfileName = currentPath + "/" + myFileController.getFileNameFromPath(filePath, true) + ".txt";
    myFileController.outputDValue(d_value_set, outputfileName);
    
    cout << "end!!" << endl;
}