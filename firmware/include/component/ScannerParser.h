#ifndef SCANNERPARSER_H
#define SCANNERPARSER_H

#include <iostream>
#include <vector>
#include <HardwareSerial.h>




class ScannerParser {
private:
    /**
     *  We will save on a buffer the data that wasnt extracted yet
     */
    std::vector<std::string> buffer;
public:
    /**
     * Returns the vector containing all read strings from the last extraction (cleans the buffer automatically)
     */
    std::vector<std::string> getData();
    
    /**
     * Initializes the Scanner
     */
    void setup(); 
   
    /**
     * Updates the state of the parser, reads whatever is in front o it and saves the data on the buffer
     */
    void update(); 
   
};


#endif