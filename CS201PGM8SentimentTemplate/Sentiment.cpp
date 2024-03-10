#include "Sentiment.h"

//SUPPORT FUNCTION TO FIND THE CURRENT WORD IN THE SENTEMENT VECTOR & RETURN THE SENTIMENT VALUE
//PRE:   accepts a string (in) and the words list
//POST:  returns the sentiment of the word if found or the value 0 if not
double sentimentVal(string in, vector<wordList> &words) {
/* Old Search
    for (auto i : words) {
        if (i.word == in)
            return i.value;
    }
    return 0;
*/


/* Psuedocode provided by CHATGPT
 * high = words.size() - 1 // Index of the last element
low = 0

while low <= high do
    mid = low + (high - low) / 2 // Calculate the middle index

    if in == words[mid].word then
        return words[mid].value // Word found, return its value
    else if in > words[mid].word then
        low = mid + 1 // Update the low index to search in the right half
    else if in < words[mid].word then
        high = mid - 1 // Update the high index to search in the left half
end while

return 0 // Word not found, return 0

 * */

// Using Binary Search: Assistance from https://www.freecodecamp.org/news/binary-search-in-c-algorithm-example/
// Initializes high and low values
int high = words.size();
int low = 0;
while (low <= high) {
    int mid = low + (high - low) / 2;

    if (in == words[mid].word) {
        return words[mid].value;
    }
    else if (in > words[mid].word) {
        low = mid + 1;
    }
    else if (in < words[mid].word) {
        high = mid - 1;
    }
}
    return 0;
}


//PRE:   accepts an empty set of 3 <wordList> vectors
//POST:  loads all three vectors with data from sentiment.txt
void loadSentiment(vector<wordList>& sentList,
    vector<wordList>& posList,
    vector<wordList>& negList) {

    /* Used ChatGPT to create Psuedocode for this function
     *
     * openFile("sentiment.txt", inFile) // Open the input file
if inFile is not open
    output "Unable to open input file"
    return
end if

while inFile is open do
    readLine(inFile, inRecord) // Read a line from the file

    try
        pos = findFirstComma(inRecord) // Find the position of the first comma

        // Extract word and value from the record
        tempWord.word = substring(inRecord, 0, pos)
        tempWord.value = convertToDouble(substring(inRecord, pos + 1))

        // Add the word to the sentList vector
        sentList.push_back(tempWord)

        // If positive enough, add to posList
        if tempWord.value > 1.5 then
            posList.push_back(tempWord)
        // If negative enough, add to negList
        else if tempWord.value < -1.5 then
            negList.push_back(tempWord)
        end if
    catch invalid_argument
        output "Ignoring: " + tempWord.word
    catch all
        output "Unknown Error - Ignoring: " + tempWord.word
    end try
end while

closeFile(inFile) // Close the input file
return
     * */


    ifstream inFile;
    //open sentiment.txt
    inFile.open("sentiment.txt");
    if (!inFile.is_open()) {
        cout << "Unable to open input file\n";
        return;
    }
    wordList tempWord;
    string inRecord;
    getline(inFile, inRecord);
    while (inFile.good()) {
        //add the word to the sentList vector
        //if positive enough add to posList
        //if negative enough add to negList
        try {
            int pos = inRecord.find(",");
            tempWord.word = inRecord.substr(0, pos);
            tempWord.value = stod(inRecord.substr(pos + 1));
            sentList.push_back(tempWord);
            if (tempWord.value > 1.5) {
                posList.push_back(tempWord);
            }
            else if (tempWord.value < -1.5) {
                negList.push_back(tempWord);
            }
        }
        catch (invalid_argument) {
            cout << "Ignoring: " << tempWord.word << endl;
        }
        catch (...) {
            cout << "Unknown Error - Ignoring : " << tempWord.word << endl;
        }
        getline(inFile, inRecord);
    }
    //close sentiment.txt
    inFile.close();
    return;
 
}

//PRE:  positive & negative word lists are sent
//POST: Open the input file & output file (infile+Output 
//      'Clean' the input by removing syntax and setting all to lower case
//       Get the sentiment value for the edited word
//        store unedited word in vector with sentiment value
//         if sentiment < -1  find a positive word to replace it
//         if sentiment > 1   find a negative word to replace

void processFile(ifstream& inFile, ofstream& outFile, string fileName,
                    vector<wordList>& words,               // vectors containing the word & sentiment
                    vector<wordList>& posList, 
                    vector<wordList>& negList) {


    // Given Pseudocode
    /* SETUP inFileName as ifstream
     * WHILE (inFILE >> origWord)
     *  LOOP through origWord and change uppercase alpha to lowercase
     *  SET editWord to this updated word
     *  SET wordValue = sentimentVal(editWord)
     *  UPDATE totalOrigSent += Value
     *  ADD word, wordValue to vector
     *  IF (wordValue > 1)
     *      GET randomNegWord
     *      ADD randomNegWord to negWords
     *  ELSE
     *      ADD origWord to negWords
     *  IF (wordValue < -1)
     *      GET randomPosWord
     *      ADD randomPosWord & value to posWords
     *  ELSE
     *      ADD origWord to posWords
     * ENDWHILE
     * PRINT vectors
     * */


    string inWord, newWord, punctuation;
    char newChar;
    // Create vectors for the outputs (original, negative, positive) and for the list of words changed (original for positive and negative, updated positive, updated negative)
    vector<wordList> origWords, negWords, posWords, negUpdated, posUpdated, negOld, posOld;
    // Track the totals for the original, negative, positive, original for words updated in both positive and negative instances, and the total of the negative and positive words that are added themselves.
    double origWordsTotal = 0;
    double negWordsTotal = 0;
    double posWordsTotal = 0;
    double negWordsUpdatedTotalOnly = 0;
    double negWordsOldTotalOnly = 0;
    double posWordsUpdatedTotalOnly = 0;
    double posWordsOldTotalOnly = 0;


    //cout << "CODE THIS\n";
    outFile << "PROCESSING FILE: " << fileName << endl << endl;

    // reset all vectors & values before processing inFile
    origWords.clear();
    negWords.clear();
    posWords.clear();

    //   read and edit the initial file
    //   remove any unnecessary characters
    while (inFile >> inWord) {
        //  the inFile read will read a word up to the next space.
        //  there may be some unwanted characters
        //  read char by char and only use alphabetic characters
        //  and change all uppercase to lowercase
        newWord = "";
        punctuation = "";
        for (int i = 0, len = inWord.size(); i < len; i++) {
            if (isalpha(inWord[i])) {
                newChar = tolower(inWord[i]);
                newWord += newChar;
            }
            //Store punctuation to a string that will be added later
            else {
                punctuation += inWord[i];
            }
        }
        cout << newWord << " : " << sentimentVal(newWord, words) << endl;
        // add the 'clean' word to a temp location & add original word & sentiment value to origWords
        wordList tempWord;
        tempWord.word = newWord;
        // Get the word's value
        tempWord.value = sentimentVal(newWord, words);
        origWordsTotal += tempWord.value;

        //Word with punctuation
        newWord += punctuation;
        wordList OrigWord;
        // Push the original word to its own vector
        OrigWord.word = newWord;
        OrigWord.value = tempWord.value;
        origWords.push_back(OrigWord);
        //CHECK TO MAKE SENTIMENT MORE NEGATIVE
        if (tempWord.value > 1) {
            // Generate random negative word and push it to its own vector
            int random = rand() % negList.size();
            wordList random_word_neg = negList[random];
            negUpdated.push_back(random_word_neg);
            // Add to total of each negative vector, tracking the overall score and the changed negative values and the values beforehand
            // Push the original and changed words to seperate vectors to be displayed later
            negWordsUpdatedTotalOnly += random_word_neg.value;
            negOld.push_back(tempWord);
            negWordsOldTotalOnly += tempWord.value;
            //Add punctuation back in to the word
            string hold = random_word_neg.word;
            hold += punctuation;
            random_word_neg.word = hold;
            negWords.push_back(random_word_neg);
            negWordsTotal += random_word_neg.value;
        }
        else {
            // If word is not positive enough don't change it
            negWords.push_back(OrigWord);
            negWordsTotal += OrigWord.value;
        }
        //store current word before checking to replace
        //add original word or replacement to negVector


        //CHECK TO MAKE SENTIMENT MORE POSITIVE
        if (tempWord.value < -1) {
            // Generate random positive word and push it to its own vector
            int random = rand() % posList.size();
            wordList random_word_pos = posList[random];
            posUpdated.push_back(random_word_pos);
            // Add to total of each positive vector, tracking the overall score and the changed positive values and the values beforehand
            // Push the original and changed words to seperate vectors to be displayed later
            posWordsUpdatedTotalOnly += random_word_pos.value;
            posOld.push_back(tempWord);
            posWordsOldTotalOnly += tempWord.value;
            //Add punctuation back in to the word
            string hold = random_word_pos.word;
            hold += punctuation;
            random_word_pos.word = hold;
            posWords.push_back(random_word_pos);
            posWordsTotal += random_word_pos.value;
        }
        else {
            // If word is not positive enough don't change it
            posWords.push_back(OrigWord);
            posWordsTotal += OrigWord.value;
        }
        //store current word before checking to replace
        //add original word or replacement to posVector
    }
    //OUTPUT UPDATES
    //EDIT ORIGINAL WORDS VECTOR FOR OUTPUT
    //check to see if the length of the words written so far
    outFile << "FORMATTED REVIEW: " << endl;
    int lineLength = 0;
    for (unsigned int i = 0; i < origWords.size(); i++) {
        lineLength += origWords[i].word.length() + 1;
        if (lineLength > 80) {
            outFile << "\n";
            lineLength = origWords[i].word.length() + 1;
        }
        outFile << origWords[i].word + " ";
    }
    outFile << endl << endl;
    outFile << "ORIGINAL SENTIMENT: " << origWordsTotal;
    outFile << endl << endl;

    //EDIT MORE NEGATIVE VECTOR FOR OUTPUT
    //CHECK THAT SOMETHING HAS BEEN UPDATED BEFORE BEGINNING PRINT
    //IF THE ORIGINAL SENTMENT & NEGATIVE SENTIMENT ARE EQUAL, PRINT "REVIEW NOT UPDATED, THE SENTIMENT REMAINS: "
    if (negWordsTotal == origWordsTotal) {
        // If sentiment is the same
        outFile << "REVIEW NOT UPDATED TO BE MORE NEGATIVE. THE SENTIMENT REMAINS: " << origWordsTotal;
        outFile << endl << endl;
    }
    else {
        // Output the orignal words and the negative replacements
        outFile << "WORDS UPDATED TO BE MORE NEGATIVE: " << endl;
        for (int i = 0; i < negUpdated.size(); i++) {
            outFile << "\t" << setw(12) <<  negOld[i].word  << "\t" << setw(6) << negOld[i].value << "\t" << setw(12) << negUpdated[i].word << "\t" << setw(6) << negUpdated[i].value << endl;
        }
        outFile << "TOTALS: " << setw(18) << negWordsOldTotalOnly << setw(24) << negWordsUpdatedTotalOnly << endl << endl;
        // Output negative review
        outFile << "UPDATED REVIEW (MORE NEGATIVE): " << endl;
        int lineLength = 0;
        for (unsigned int i = 0; i < negWords.size(); i++) {
            lineLength += negWords[i].word.length() + 1;
            if (lineLength > 80) {
                outFile << "\n";
                lineLength = negWords[i].word.length() + 1;
            }
            outFile << negWords[i].word + " ";
        }
        outFile << endl << endl;
        // Output new sentiment
        outFile << "UPDATED NEGATIVE SENTIMENT: " << negWordsTotal;
        outFile << endl << endl;
    }



    //EDIT MORE POSITIVE VECTOR FOR OUTPUT
    //CHECK THAT SOMETHING HAS BEEN UPDATED BEFORE BEGINNING PRINT
    //IF THE ORIGINAL SENTMENT & NEW POSITIVE SENTIMENT ARE EQUAL, PRINT "REVIEW NOT UPDATED, THE SENTIMENT REMAINS: "
    if (posWordsTotal == origWordsTotal) {
        // If sentiment is the same
        outFile << "REVIEW NOT UPDATED TO BE MORE POSITIVE. THE SENTIMENT REMAINS: " << origWordsTotal;
        outFile << endl << endl << endl;
    }
    else {
        // Output original words and the positive counterparts
        outFile << "WORDS UPDATED TO BE MORE POSITIVE: " << endl;
        for (int i = 0; i < posUpdated.size(); i++) {
            outFile << "\t" << setw(12) <<  posOld[i].word << "\t" << setw(6) << posOld[i].value << "\t" << setw(12) << posUpdated[i].word << "\t" << setw(6) << posUpdated[i].value << endl;
        }
        outFile << "TOTALS: " << setw(18) << posWordsOldTotalOnly << setw(24) << posWordsUpdatedTotalOnly << endl << endl;
        // Output positive review
        outFile << "UPDATED REVIEW (MORE POSITIVE): " << endl;
        int lineLength = 0;
        for (unsigned int i = 0; i < posWords.size(); i++) {
            lineLength += posWords[i].word.length() + 1;
            if (lineLength > 80) {
                outFile << "\n";
                lineLength = posWords[i].word.length() + 1;
            }
            outFile << posWords[i].word + " ";
        }
        outFile << endl << endl;
        // Output new sentiment
        outFile << "UPDATED POSITIVE SENTIMENT: " << posWordsTotal;
        outFile << endl << endl;
    }
 
 
}

// Didn't need to use the function below
void processFile(ifstream inFile, ofstream outFile, vector<wordList> sentWordList, vector<wordList> posList, vector<wordList> negList) {
    /* SETUP inFileName as ifstream
     * WHILE (inFILE >> origWord)
     *  LOOP through origWord and change uppercase alpha to lowercase
     *  SET editWord to this updated word
     *  SET wordValue = sentimentVal(editWord)
     *  UPDATE totalOrigSent += Value
     *  ADD word, wordValue to vector
     *  IF (wordValue > 1)
     *      GET randomNegWord
     *      ADD randomNegWord to negWords
     *  ELSE
     *      ADD origWord to negWords
     *  IF (wordValue < -1)
     *      GET randomPosWord
     *      ADD randomPosWord & value to posWords
     *  ELSE
     *      ADD origWord to posWords
     * ENDWHILE
     * PRINT vectors
     * */
}