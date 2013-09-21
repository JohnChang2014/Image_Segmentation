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

/*
 * Class: FileController
 *        handle the process of input and output data
 */
class FileController {
private:
    ofstream myfile;
    string outputfile, inputfile;
    
    /*
     * Method Member: splitString(string, char, int)
     *     receives a char delimiter; returns a vector of strings
     *
     * Parameters:
     *     string data    - a string to be split
     *     char delim     - a delimiter helping split a string into several parts
     *     int rep        - by default ignores repeated delimiters, unless argument rep == 1
     *
     * Return:
     *     vector<string> - a vector includes all parameters.
     */
    vector<string> splitString(string data, char delim, int rep=0) {
        vector<string> flds;
        if (!flds.empty()) flds.clear();  // empty vector if necessary
        
        string buf = "";
        int i = 0;
        while (i < data.length()) {
            if (data[i] != delim) buf += data[i];
            else if (rep == 1) {
                flds.push_back(buf);
                buf = "";
            } else if (buf.length() > 0) {
                flds.push_back(buf);
                buf = "";
            }
            i++;
        }
        if (!buf.empty()) flds.push_back(buf);
        return flds;
    }
    
public:
    
    FileController() {
        this->outputfile = "./result.txt";
    }
    
    FileController(string path) {
        this->outputfile = path + "result.txt";
    }
    
    /*
     * Method Member: getCurrentPath()
     *     find out current path of this program.
     *
     * Return:
     *     string value - a current path.
     */
    string getCurrentPath() {
        char cCurrentPath[FILENAME_MAX];
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) return "./";
        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
        return cCurrentPath;
    }
    
    /*
     * Method Member: toString(int)
     *     convert an integer into a string
     *
     * Parameters:
     *     int i  - an integer to be converted
     *
     * Return:
     *     string - a string from an integer
     *
     */
    string toString(int i) {
        std::string s;
        std::stringstream out;
        out << i;
        s = out.str();
        return s;
    }
    
    /*
     * Method Member: getFileNameFromPath(string, bool)
     *     get a filename from the string of path
     *
     * Parameters:
     *     string filename     - a file path including path and the name of a file
     *     bool isNonExtension - remove extension of a file or not (default is false)
     *
     * Return:
     *     string - a filename.
     */
    string getFileNameFromPath(string filename, bool isNonExtension=false) {
        // Remove directory if present.
        // Do this before extension removal incase directory has a period character.
        const size_t last_slash_idx = filename.find_last_of("\\/");
        if (std::string::npos != last_slash_idx) {
            filename.erase(0, last_slash_idx + 1);
        }
        
        // Remove extension if present.
        if (isNonExtension) {
            const size_t period_idx = filename.rfind('.');
            if (std::string::npos != period_idx) {
                filename.erase(period_idx);
            }
        }
        return filename;
    }
    
    /*
     * Method Member: requestInputs()
     *     show info on console and request user to enter data.
     *
     * Return:
     *     vector<string>  - a vector including more than one paths of files
     */
    vector<string> requestInputs() {
        string input_string;
        vector<string> results;
        cout << "This program is designed to segment images in RGB color space." << endl;
        cout << "You can enter one image as the inputs with its size r and c" << endl;
        cout << "You also can assign two specific values as the K values, k-start and k-end." << endl;
        cout << "If you assign the k values, this program will produce more than one output file for each K." << endl << endl;
        cout << "Usage:" << endl;
        cout << "    filepath r c [k-start] [k-end]" << endl << endl;
        cout << "        filepath     a file path including image file name" << endl;
        cout << "            r        the height of the input image" << endl;
        cout << "            c        the width of the input image" << endl;
        cout << "        [k-start]    a value showing how many clusters you want, always smaller than k-end" << endl;
        cout << "        [k-end]      a value showing how many clusters you want, always greater than k-end" << endl << endl;
        cout << "ex:" << endl;
        cout << "    ./test/tiger1.raw 461 690 2 10" << endl << endl;
        do {
            cout << "Now enter your inputs: ";
            getline(cin, input_string);
            
            // determines how many images in the user input.
            cout << "your input is " << input_string << endl;
            char delim = ' ';
            results = this->splitString(input_string, delim);
        } while(isValid(results));
        return results;
    }
    
    /*
     * Method Member: isValid(vector<vector<string> >)
     *     check if input parameter are valid or not
     *
     * Parameters:
     *     vector<string>  results  - a vector including all parameter the program needs
     *
     * Return:
     *     bool                     - true: invalid, false: valid
     */
    bool isValid(vector<string> results) {
        
        int j = (int) results.size();
        if (j < 3) {
            cout << "You are missing required parameters filename, height, or width." << endl;
            cout << "Please enter your inputs again." << endl;
            return true;
        }
        if (j >= 4) {
            int start_k = atoi(results.at(3).c_str());
            if (start_k < 2) {
                cout << "The parameter 'start_k' must be greater than or equal to two." << endl;
                cout << "Please enter your inputs again." << endl;
                return true;
            }
        }
        if (j == 5) {
            int start_k = atoi(results.at(3).c_str());
            int end_k   = atoi(results.at(4).c_str());
            if (start_k > end_k) {
                cout << "The parameter 'start_k' must be less than or equal to the parameter 'end_k'." << endl;
                cout << "Please enter your inputs again." << endl;
                return true;
            }
        }
    
        return false;
    }
    
    /*
     * Method Member: readFile(string, int, int)
     *     handle reading input data and store all data into a vector.
     *
     * Parameters:
     *     string fileName - a file path including path and the name of an input file.
     *     int    row      - the number of rows in an image, namely height of the image
     *     int    col      - the number of columns in an image, namely width of the image
     *
     * Return:
     *     vector<vector<pixel> > - a vector stores information about pixels, such as coordinates, color space, and group
     */
    vector<vector<pixel> > readFile(string fileName, int row=64, int col=64) {
        int color_type = 0;
        int pixel_value, n_bytes;
        unsigned char tmp_value;
        ifstream inFile;
        
        this->inputfile = fileName;
        
        inFile.open(fileName.c_str(), ios::in | ios::binary);
        if (!inFile) {
            cout << "Unable to open file";
            exit(1); // terminate with error
        }
        
        vector<vector<pixel> > tmpVec(row, vector<pixel> (col));
        n_bytes = 0;
        
        // start to read raw image
        for (int n=0; n< row; n++) {
            for (int m=0; m< col; m++) {
                pixel data_point;
                
                // read RGB values
                for (int k = 0; k < 3; k++) {
                    tmp_value   = inFile.get();
                    pixel_value = tmp_value;            // casting unsigned char into integer
                    color_type  = n_bytes % 3;
            
                    // store RGB values
                    if (color_type == 0) data_point.color_space.R = pixel_value;
                    if (color_type == 1) data_point.color_space.G = pixel_value;
                    if (color_type == 2) data_point.color_space.B = pixel_value;
                    n_bytes++;
                }
                
                // store info about coordinates of pixel
                data_point.position.r = n;
                data_point.position.c = m;
                
                // set pixel to zero, which means it doesn't belong to any group
                data_point.group = 0;
                tmpVec[n][m] = data_point;
                
            }
        }
        return tmpVec;
    }
    
    /*
     * Method Member: outputRAW(vector<vector<pixel> >, string)
     *     write all RGB values of pixels into a RAW file
     *
     * Parameters:
     *     vector<vector<pixel> > imgVec - a vector stores information about pixels, such as coordinates, color space, and group
     *     string fileName               - a file path including path and the name of an input file.
     *
     * Return:
     *     bool                          - true: succeed, false: fail
     */
    bool outputRAW(vector<vector<pixel> > &imgVec, string fileName="testoutput.raw") {
        ofstream outputfile;
        char tmp_value;
        int row         = (int) imgVec.size();
        int col         = (int) imgVec[0].size();

        outputfile.open (fileName.c_str(), ios::out | ios::binary);
        
        if (outputfile.is_open()) {
        
            // write all RGB values of pixels into RAW file
            for (int n=0; n< row; n++) {
                for (int m=0; m< col; m++) {
                    tmp_value = imgVec[n][m].color_space.R;
                    outputfile.write(&tmp_value, sizeof(tmp_value));
                    tmp_value = imgVec[n][m].color_space.G;
                    outputfile.write(&tmp_value, sizeof(tmp_value));
                    tmp_value = imgVec[n][m].color_space.B;
                    outputfile.write(&tmp_value, sizeof(tmp_value));
                }
            }
        } else {
            cout << "Error: cannot open the file! " << fileName << endl;
            exit(1);
        }
        outputfile.close();
        return true;
    }
    
    /*
     * Method Member: outputDValue(vector<vector<tdist> >, int, string)
     *     handle how to print out the result into a text file.
     *
     * Parameters:
     *     vector<vector<int> > d_value_set - a vector including D value for each K value
     *     string fileName                  - a file name including path and the name of an output file.
     *
     * Return:
     *     bool               - true: succeed, false: fail
     */
    bool outputDValue(vector<vector<int> > d_value_set, string fileName="testoutput.txt") {
        string line;
        
        this->myfile.open (fileName.c_str(), ios::out);
        if (this->myfile.is_open()) {
            int n_results = (int) d_value_set.size();
            this->myfile << "Result\n";
            this->myfile << "-------------------------------\n";
            for (int n=0; n<n_results; n++) {
                line = toString(n + 2) + "\t" + toString(d_value_set[n].at(0)) + "\t" + toString(d_value_set[n].at(1));
                this->myfile << line << "\n";
            }
            this->myfile << "-------------------------------\n";
        } else {
            cout << "Unable to open file" << endl;
            exit(1);
        }
        this->myfile.close();
        return true;
    }
};