// CS201PGM8Sentiment 
// NAME:  

#include "Sentiment.h"
using namespace std;

int main()
{
    srand((unsigned) time(NULL)); // Suggested by https://www.digitalocean.com/community/tutorials/random-number-generator-c-plus-plus to get more random words at a quicker interval
    vector<wordList> sentList, posList, negList;

    // load vectors 
    loadSentiment(sentList, posList, negList);

    // open output file
    ofstream outFile;
    outFile.open("outputReport.txt");
    if (!outFile.is_open()) {
        cout << "Unable to open output file\n";
        return 1;
    }
    // open review files

    string fileName;
    for (unsigned int i = 1; i < 9; i++) {
        // open input file adding to_string(i) + ".txt" to review
        string fileName = "review" + to_string(i) + ".txt";
        ifstream inFile;
        inFile.open(fileName);
        // if not able to open, print a message and continue
        if (!inFile.is_open()) {
            cout << "Error Opening File: " << fileName << endl;
            continue;
        }
        // else process the file & close it
        processFile(inFile, outFile, fileName, sentList, posList, negList);
        inFile.close();
    }

    //close the output file
    outFile.close();
}
