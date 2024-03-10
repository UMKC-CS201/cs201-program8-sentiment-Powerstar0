// CS201PGM8Sentiment 
// NAME: Johnny Diep
// Date: 03/09/2024
// Purpose: To evaluate the sentiment value of different reviews and change the review to both make it more positive and negative, printing out both the review, original and changed words,
//and the changed sentiment value each time. The Program Does a binary search to find the associated sentiment value of a word.
//The program also maintains punctuation each time.

#include "Sentiment.h"
using namespace std;

int main()
{
    /* Psuedocode created by CHATGPT
     * // Initialize random number generator
srand(time(NULL))

// Initialize vectors for sentiment analysis
sentList = empty vector
posList = empty vector
negList = empty vector

// Load sentiment lists
loadSentiment(sentList, posList, negList)

// Open output file for writing
outFile = openFile("outputReport.txt", write)
if outFile is not open then
    output "Unable to open output file"
    return 1
end if

// Process review files
for i = 1 to 8 do
    fileName = "review" + toString(i) + ".txt"
    inFile = openFile(fileName, read)
    if inFile is not open then
        output "Error Opening File: " + fileName
        continue // Skip to next iteration if unable to open file
    end if

    // Process the file
    processFile(inFile, outFile, fileName, sentList, posList, negList)

    closeFile(inFile) // Close the input file
end for

closeFile(outFile) // Close the output file
     * */


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
