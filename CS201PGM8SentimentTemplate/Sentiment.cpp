#include "Sentiment.h"

//SUPPORT FUNCTION TO FIND THE CURRENT WORD IN THE SENTEMENT VECTOR & RETURN THE SENTIMENT VALUE
//PRE:   accepts a string (in) and the words list
//POST:  returns the sentiment of the word if found or the value 0 if not
double sentimentVal(string in, vector<wordList> &words) {

    for (auto i : words) {
        if (i.word == in)
            return i.value;
    }
    return 0;

}


//PRE:   accepts an empty set of 3 <wordList> vectors
//POST:  loads all three vectors with data from sentiment.txt
void loadSentiment(vector<wordList>& sentList,
    vector<wordList>& posList,
    vector<wordList>& negList) {
    ifstream inFile;
    //open sentiment.txt
    inFile.open("/Users/johnnydiep/CLionProjects/cs201-program8-sentiment-Powerstar0/CS201PGM8SentimentTemplate/sentiment.txt");
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
 
    string inWord, newWord;
    char newChar;
    vector<wordList> origWords, negWords, posWords;
    double origWordsTotal = 0;
    double negWordsTotal = 0;
    double posWordsTotal = 0;

    cout << "CODE THIS\n";
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
        for (int i = 0, len = inWord.size(); i < len; i++) {
            if (isalpha(inWord[i])) {
                newChar = tolower(inWord[i]);
                newWord += newChar;
            }
        }
        cout << newWord << " : " << sentimentVal(newWord, words) << endl;
        // add the 'clean' word to a temp location & add original word & sentiment value to origWords
        string clean_word = newWord;
        wordList tempWord;
        tempWord.word = newWord;
        tempWord.value = sentimentVal(newWord, words);
        origWordsTotal += tempWord.value;
        origWords.push_back(tempWord);
        //CHECK TO MAKE SENTIMENT MORE NEGATIVE
        if (tempWord.value > 1) {
            srand(time(0));
            int random = rand() % negList.size();
            wordList random_word_neg = negList[random];
            negWords.push_back(random_word_neg);
            negWordsTotal += random_word_neg.value;
        }
        else {
            negWords.push_back(tempWord);
            negWordsTotal += tempWord.value;
        }
        //store current word before checking to replace
        //add original word or replacement to negVector


        //CHECK TO MAKE SENTIMENT MORE POSITIVE
        if (tempWord.value < 1) {
            srand(time(0));
            int random = rand() % posList.size();
            wordList random_word_pos = posList[random];
            posWords.push_back(random_word_pos);
            posWordsTotal += random_word_pos.value;
        }
        else {
            posWords.push_back(tempWord);
            posWordsTotal += tempWord.value;
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
    outFile << "UPDATED REVIEW (MORE NEGATIVE): " << endl;
    for (unsigned int i = 0; i < negWords.size(); i++) {
        lineLength += negWords[i].word.length() + 1;
        if (lineLength > 80) {
            outFile << "\n";
            lineLength = negWords[i].word.length() + 1;
        }
        outFile << negWords[i].word + " ";
    }
    //CHECK THAT SOMETHING HAS BEEN UPDATED BEFORE BEGINNING PRINT
    //IF THE ORIGINAL SENTMENT & NEGATIVE SENTIMENT ARE EQUAL, PRINT "REVIEW NOT UPDATED, THE SENTIMENT REMAINS: "
    outFile << endl << endl;
    outFile << "NEGATIVE SENTIMENT: " << negWordsTotal;
    outFile << endl << endl;


    //EDIT MORE POSITIVE VECTOR FOR OUTPUT
    outFile << "UPDATED REVIEW (MORE POSITIVE): " << endl;
    for (unsigned int i = 0; i < posWords.size(); i++) {
        lineLength += posWords[i].word.length() + 1;
        if (lineLength > 80) {
            outFile << "\n";
            lineLength = posWords[i].word.length() + 1;
        }
        outFile << posWords[i].word + " ";
    }
    //CHECK THAT SOMETHING HAS BEEN UPDATED BEFORE BEGINNING PRINT
    //IF THE ORIGINAL SENTMENT & NEW POSITIVE SENTIMENT ARE EQUAL, PRINT "REVIEW NOT UPDATED, THE SENTIMENT REMAINS: "
    outFile << endl << endl;
    outFile << "POSITIVE SENTIMENT: " << posWordsTotal;
    outFile << endl << endl << endl;
 
 
}

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