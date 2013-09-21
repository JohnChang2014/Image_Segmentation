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
 * Class: Cluster
 *        handle the process of input and output data
 */
class Cluster {
private:
    vector<mean> init_mean_sets;
    vector<vector<mean> > mean_sets;
    vector<int> d_results;
    
    /*
     * Method Member: assignGroup(color, int, int)
     *     determine which group the pixel should be assigned to
     *
     * Parameters:
     *     color pixel_value     - a struct including RGB values
     *     int iteration         - a number pointing out which iteration the program is in
     *     int    k              - a number meaning how many cluster the program should compare with pixels
     *
     * Return:
     *     int                   - a final number about which cluster the pixel is assigned to
     *
     */
    int assignGroup(color pixel_value, int iteration, int k) {
        int decision = 1;
        int min      = 0;
        int dist     = 0;
        for (int n=0; n < k; n++) {
            dist += abs(pixel_value.R - mean_sets.at(iteration).at(n).r);
            dist += abs(pixel_value.G - mean_sets.at(iteration).at(n).g);
            dist += abs(pixel_value.B - mean_sets.at(iteration).at(n).b);
            dist = dist / 3;
            if (n == 0) min = dist;
            if (dist <= min) {
                min      = dist;
                decision = (n + 1);
            }
        }
        return decision;
    }
    
    /*
     * Method Member: rndup(double, int)
     *     round up a double type and show it into some decimal place
     *
     * Parameters:
     *     double n - a doulbe value you would like to round up
     *     int prec - how many specific decimal places you need
     *
     * Return:
     *     double
     *
     */
    void updateMeanValues(vector<vector<pixel> > &imgVec, int k, int iteration) {
        int row        = (int) imgVec.size();
        int col        = (int) imgVec[0].size();
        vector<mean> next_means_set;
        vector<int> n_members;
        vector<tcolor> totals;
        
        // initialize empty mean_set of each cluster for next iteration
        for (int n=0; n < k; n++) {
            mean tmp_meanValue;
            tcolor tmp_tcolor;
            tmp_meanValue.r = 0;
            tmp_meanValue.g = 0;
            tmp_meanValue.b = 0;
            tmp_tcolor.tr = 0;
            tmp_tcolor.tg = 0;
            tmp_tcolor.tb = 0;
            next_means_set.push_back(tmp_meanValue);
            n_members.push_back(0);
            totals.push_back(tmp_tcolor);
        }
        
        // sum up color value of R, G, and B color component for each cluster
        int cluster_n = 0;
        for (int n=0; n< row; n++) {
            for (int m=0; m< col; m++) {
                cluster_n = imgVec[n][m].group - 1;
                totals.at(cluster_n).tr += imgVec[n][m].color_space.R;
                totals.at(cluster_n).tg += imgVec[n][m].color_space.G;
                totals.at(cluster_n).tb += imgVec[n][m].color_space.B;
                n_members.at(cluster_n)++ ;
            }
        }
        
        // calculates mean value of each cluster
        
        for (int n=0; n < k; n++) {
            int members_n = n_members.at(n);
            if (members_n == 0) {
                next_means_set.at(n).r = mean_sets.at(iteration).at(n).r;
                next_means_set.at(n).g = mean_sets.at(iteration).at(n).g;
                next_means_set.at(n).b = mean_sets.at(iteration).at(n).b;
            } else {
                next_means_set.at(n).r = totals.at(n).tr / members_n;
                next_means_set.at(n).g = totals.at(n).tg / members_n;
                next_means_set.at(n).b = totals.at(n).tb / members_n;
            }
        }
        
        mean_sets.push_back(next_means_set);
    }
    
    /*
     * Method Member: assignColorValues(vector<vector<pixel> >)
     *     assign mean values from final result of clustering to pixels
     *
     * Parameters:
     *     vector<vector<pixel> > imgVec - a vector stores information about pixels, such as coordinates, color space, and group
     *
     */
    void assignColorValues(vector<vector<pixel> > &imgVec) {
        int row            = (int) imgVec.size();
        int col            = (int) imgVec[0].size();
        int last_iteration = (int) mean_sets.size() - 1;
        int cluster_n      = 0;
        for (int n=0; n< row; n++) {
            for (int m=0; m< col; m++) {
                cluster_n = imgVec[n][m].group - 1;
                imgVec[n][m].color_space.R = mean_sets.at(last_iteration).at(cluster_n).r;
                imgVec[n][m].color_space.G = mean_sets.at(last_iteration).at(cluster_n).g;
                imgVec[n][m].color_space.B = mean_sets.at(last_iteration).at(cluster_n).b;
            }
        }
    }
    
    /*
     * Method Member: isConverge(int)
     *     check if algorithm goes to convergence or not
     *
     * Parameters:
     *     int iteration    - a value meaning what iteration the program is in
     *
     * Return:
     *     bool             - true: converge,  false: not converge
     *
     */
    bool isConverge(int iteration) {
        int n_equal = 0;
        int k = (int) mean_sets[0].size();
        for (int n = 0; n < k; n++) {
            if (mean_sets.at(iteration-1).at(n).r != mean_sets.at(iteration).at(n).r) break;
            if (mean_sets.at(iteration-1).at(n).g != mean_sets.at(iteration).at(n).g) break;
            if (mean_sets.at(iteration-1).at(n).b != mean_sets.at(iteration).at(n).b) break;
            n_equal++;
        }
        if (n_equal == k) return true;
        return false;
    }
    
    /*
     * Method Member: computeDValue(vector<vector<pixel> >)
     *     round up a double type and show it into some decimal place
     *
     * Parameters:
     *     vector<vector<pixel> > imgVec - a vector stores information about pixels, such as coordinates, color space, and group
     *
     */
    void computeDValue(vector<vector<pixel> > &imgVec) {
        vector<tdist> d_values;
        vector<mean> last_result = mean_sets.back();
        int d_result_original    = 0;
        int d_result_normal      = 0;
        int k                    = (int) last_result.size();
        int row                  = (int) imgVec.size();
        int col                  = (int) imgVec[0].size();
        int cluster_n            = 0;
        for (int n=0; n < k; n++) {
            tdist tmp;
            tmp.tdr = 0;
            tmp.tdg = 0;
            tmp.tdg = 0;
            tmp.n_members = 0;
            d_values.push_back(tmp);
        }
        
        for (int n=0; n< row; n++) {
            for (int m=0; m< col; m++) {
                cluster_n = imgVec[n][m].group - 1;
                double tdr = abs(imgVec[n][m].color_space.R - last_result.at(cluster_n).r);
                double tdg = abs(imgVec[n][m].color_space.G - last_result.at(cluster_n).g);
                double tdb = abs(imgVec[n][m].color_space.B - last_result.at(cluster_n).b);
                d_values[cluster_n].tdr += (int) pow(tdr, 2);
                d_values[cluster_n].tdg += (int) pow(tdg, 2);
                d_values[cluster_n].tdb += (int) pow(tdb, 2);
                d_values[cluster_n].n_members++;
            }
        }
        
        for (int n=0; n < k; n++) {
            if (d_values[n].n_members == 0) {
                cout << ">> empty group" << endl;
            } else {
                int  group_error = d_values[n].tdr + d_values[n].tdg + d_values[n].tdb;
                d_values[n].original           = group_error;
                d_values[n].normal             = (group_error / d_values[n].n_members);
                d_result_original             += d_values[n].original;
                d_result_normal               += d_values[n].normal;
                cout << ">> " << d_values[n].original << "," << d_values[n].normal << endl;
            }
        }
        d_result_normal = d_result_normal / k;
        cout << "(LSE, normalized LSE) = (" << d_result_original << "," << d_result_normal << ")" << endl << endl;
        this->d_results.push_back(d_result_original);
        this->d_results.push_back(d_result_normal);
    }
    
public:
    
    Cluster() {}
    
    vector<int> getDValues() {
        return d_results;
    }
    
    /*
     * Method Member: runKMeanAlgorithm(vector<vector<pixel> >, int)
     *     perform Algorithm to get results
     *
     * Parameters:
     *     vector<vector<pixel> >  imgVec   - a vector stores information about pixels, such as coordinates, color space, and group
     *     int                      k       - how many clusters you want
     *
     */
    void runKMeanAlgorithm(vector<vector<pixel> > &imgVec, int k) {
        initializeMeanValues(k);
        executeKMean(imgVec, k);
    }
    
    /*
     * Method Member: executeKMean(vector<vector<pixel> >, int)
     *     perform Algorithm to get results
     *
     * Parameters:
     *     vector<vector<pixel> >  imgVec   - a vector stores information about pixels, such as coordinates, color space, and group
     *     int                      k       - how many clusters you want
     *
     */
    void executeKMean(vector<vector<pixel> > &imgVec, int k) {
        int row        = (int) imgVec.size();
        int col        = (int) imgVec[0].size();
        int iteration  = 0;
        int group_num  = 0;
        
        do {
            for (int n=0; n < row; n++) {
                for (int m=0; m < col; m++) {
                    group_num = assignGroup(imgVec[n][m].color_space, iteration, k);
                    imgVec[n][m].group = group_num;
                }
            }
            // update mean value of each cluster for next iteration
            updateMeanValues(imgVec, k, iteration);
            iteration++;
        } while (!isConverge(iteration));
        
        // compute Least Distance for each group.
        cout << "k = " << k << endl;
        computeDValue(imgVec);
        
        // make each pixel has new color value derived from the group it belongs to
        assignColorValues(imgVec);
    }
    
    /*
     * Method Member: initializeMeanValues(int)
     *     initialize k values as the mean values for k groups
     *
     * Parameters:
     *     int k  - how many clusters you want
     *
     */
    void initializeMeanValues(int k) {
        int j;
        float mean_value;
        mean tmp_mean;
        this->init_mean_sets.clear();
        this->mean_sets.clear();
        this->d_results.clear();
        for (j=1; j <= k; j++)  {
            mean_value = (256 / floor(k + 1)) * j;
            tmp_mean.r = mean_value;
            tmp_mean.g = mean_value;
            tmp_mean.b = mean_value;
            this->init_mean_sets.push_back(tmp_mean);
        }
        this->mean_sets.push_back(init_mean_sets);
    }
    
};