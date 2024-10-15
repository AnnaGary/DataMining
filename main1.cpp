#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

using namespace std;

void readInput(const string& filename, int& numFrames, vector<int>& addresses) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Unable to open file: \n" << filename;
        exit(1);
    }

    infile >> numFrames;
    int address;
    while (infile >> address) {
        addresses.push_back(address / 100);
    }
    infile.close();
}

void printAddys(const vector<int>& addresses) {
    cout << "The address sequence indicated in the file, 10 addresses per line: " << endl;
    for (size_t i = 0; i < addresses.size(); ++i) {
        cout << setw(4) << addresses[i];
        if ((i + 1) % 10 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

vector<int> referenceStringGen(const vector<int>& addresses) {
    vector<int> referenceString;
    for (int address : addresses) {
        referenceString.push_back(address);
    }
    return referenceString;
}

int findOptimalPageReplacement(const vector<int>& referenceString, vector<int>& frames, int numFrames, int currentIndex) {
    int furthestIndex = -1;
    int pageToReplace = -1;

    for (int i = 0; i < numFrames; ++i) {
        auto it = find(referenceString.begin() + currentIndex, referenceString.end(), frames[i]);
        if (it == referenceString.end()) {
            return i;
        }
        int index = distance(referenceString.begin(), it);
        if (index > furthestIndex) {
            furthestIndex = index;
            pageToReplace = i;
        }
    }
    return pageToReplace;
}

void optimalPageReplacement(const vector<int>& refString, int numFrames) {
    vector<int> frames(numFrames, -1);
    int pageFaults = 0;
    int lruFaults = 0;

    bool switchedToLRU = false;

    cout << "Now running the Optimal Page Replacement Algorithm..." << endl;
    cout << "Below are the page allocations to the frames." << endl;
    cout << "-------------------------------------------" << endl;

    for (size_t i = 0; i < refString.size(); ++i) {
        int page = refString[i];
        bool pgFound = false;

        for (int j = 0; j < numFrames; ++j) {
            if (frames[j] == page) {
                pgFound = true;
                break;
            }
        }

        if (!pgFound) {
            pageFaults++;
            int emptyFrameIndex = -1;
            for (int j = 0; j < numFrames; ++j) {
                if (frames[j] == -1) {
                    emptyFrameIndex = j;
                    break;
                }
            }

            if (emptyFrameIndex != -1) {
                frames[emptyFrameIndex] = page;
            }
            else {
                int farest = -1;
                int replacementPg = -1;

                for (int j = 0; j < numFrames; ++j) {
                    int future = -1;
                    for (size_t k = i + 1; k < refString.size(); ++k) {
                        if (frames[j] == refString[k]) {
                            future = k;
                            break;
                        }
                    }
                    if (future == -1) {
                        replacementPg = j; 
                        break;
                    }
                    else if (future > farest) {
                        farest = future;
                        replacementPg = j;
                    }
                }
                frames[replacementPg] = page;
            }
        }

        for (int frame : frames) {
            cout << frame << " ";
        }
        std::cout << std::endl;
       
        if (pageFaults > threshold) {
            switchedToLRU = true;
            break;
        }
    }

    cout << "Total number of page faults using Optimal: " << pageFaults << endl;

    if (switchedToLRU) {
        cout << "Total number of page faults using LRU: " << lruFaults << endl;
    }
}

int lruPageReplacement(const std::vector<int>& referenceString, int numFrames) {
    std::vector<int> frames(numFrames, -1);
    int lruFaults = 0;

    for (size_t i = 0; i < referenceString.size(); ++i) {
        int page = referenceString[i];
        bool pageFound = false;

        for (int j = 0; j < numFrames; ++j) {
            if (frames[j] == page) {
                pageFound = true;
                break;
            }
        }

        if (!pageFound) {
            lruFaults++;
            int emptyFrameIndex = -1;
            for (int j = 0; j < numFrames; ++j) {
                if (frames[j] == -1) {
                    emptyFrameIndex = j;
                    break;
                }
            }

            if (emptyFrameIndex != -1) {
                frames[emptyFrameIndex] = page;
            }
            else {
                //
            }
        }

        for (int frame : frames) {
            cout << frame << " ";
        }
        cout << endl;
    }

    return lruFaults;
}

int main() {
    int frameNums;
    vector<int> addresses;

    readInput("24F_COP4610_Project2_InputFile.txt", frameNums, addresses);

    printAddys(addresses);

    vector<int> refString = referenceStringGen(addresses);
    cout << "The reference string is: ";
    for (int ref : refString) {
        cout << ref << " ";
    }
    cout << endl;

    optimalPageReplacement(refString, frameNums);

    cout << "Thanks for using this program, dear Memory Manager!\n";
    return 0;
}