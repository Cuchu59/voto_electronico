#ifndef SCANNERPARSER_H
#define SCANNERPARSER_H

#include <iostream>
#include <vector>
#include <HardwareSerial.h>
/**
 *========================== BEFORE STARTING ===============================
 *
 *  IMPORTANT: REMEMBER SETTING UP THE SCANNER GM-865
 *  with the QRs at:
 *  - https://uelectronics.com/wp-content/uploads/2022/03/GM65-Barcode-reader-mudule-User-Manual.pdf?srsltid=AfmBOorj0S2oTTfyFHlL7l1btVdD5KN0gn8XelAxCHjNg_r0j72fyTK8
 *  
 *  or watch the video
 *  - https://www.youtube.com/watch?v=RvjQS2qywEw&t=341s
 * 
 *  SCAN: THIS: QRS:
 *  -> Series Output
 *  -> 9600bps(Default): "if it's the Serial you are using, else choose one of the others"
 * 
 * 
 *                                      You're welcome
 *                                                                               -Cuchu
 */


/**
 * 
 */
class ScannerParser {
private:
    /**
     *  We will save on a buffer the data that wasnt extracted yet
     */
    std::vector<std::string> buffer;
    
    /**
     * Place to save the current line 
     */
    std::string currentLine;

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