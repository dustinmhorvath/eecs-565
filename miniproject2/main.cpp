/*
 * Dustin Horvath
 * 2729265
 * ViginereCipher Miniproject 1
 * EECS 565
 * 2/15/16
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <ctime>


bool isZs(char* key, int keylength){
  bool notzs = true;
  for(int i = 0; i < keylength; i++){
    if(key[i] != 'Z'){
      notzs = false;
    }
  }
  return notzs;
}

char* keyInc(char* key, int keylength){
  key[keylength - 1] = static_cast<char>(key[keylength - 1] + 1);

  for(int i = keylength - 1; i > 0; i--){
    if(key[i] > 'Z'){
      key[i] = static_cast<char>(key[i] - 26);
      key[i-1] = static_cast<char>(key[i-1] + 1);
    }
  }

  return key;
}


class VigenereCipher {

public:
  std::string key;
  std::map<std::string, int> m;
  std::queue<std::string> possiblekeys;
//  std::map<std::string, int> darray[15];

  void storeDict(std::string dictionaryFile){
    std::ifstream file(dictionaryFile);
    std::string line;

    int i = 1;

    while(std::getline(file, line)){
      std::stringstream lineStream(line);
      std::string value;
      while(lineStream >> value){
        m.insert( std::pair<std::string,int>(value,i) );
      }
      i++;
    }
  }

  VigenereCipher(std::string dictionaryFile){
    storeDict(dictionaryFile);
    setKey(key);
  }

  void setKey(std::string key){
    // Force key to uppercase alphanumeric
    for(int i = 0; i < key.size(); ++i){
      if(key[i] >= 'A' && key[i] <= 'Z'){
        this -> key += key[i];
      }
      else if(key[i] >= 'a' && key[i] <= 'z'){
        this -> key += key[i] + 'A' - 'a';
      }
    }

    this -> key = key;
  }

  std::string encrypt(std::string text){
    std::string encrypted;

    // Loop across plaintext chars
    for(int i = 0, j = 0; i < text.length(); ++i){
      // Read input character at index i
      char plain = text[i];

      // Force characters to UP
      if(plain <= 'z' && plain >= 'a'){
        plain = plain + 'A' - 'a';
      }
      // Discard non-alphabetic characters 
      else if(plain < 'A' || plain > 'Z'){
        continue;
      }

      // Use viginere algorithm
      encrypted += (plain + key[j] - 2*'A') % 26 + 'A';
      // Build out return string
      j = (j + 1) % key.length();
    }
    return encrypted;
  }

  std::string decrypt(std::string text){
    std::string decrypted;

    // Loop across ciphertext chars
    for(int i = 0, j = 0; i < text.length(); ++i){
      // Read in a ciphertext character
      char ciph = text[i];
      // Force to uppercase
      if(ciph >= 'a' && ciph <= 'z'){
        ciph += 'A' - 'a';
      }
      /*
       * Discard non-alphabetic chars.
       * This is helpful because it hides word breaks and punctuation.
       */
      else if(ciph < 'A' || ciph > 'Z'){
        continue;
      }

      // Reverse of viginere from above
      decrypted += (ciph - key[j] + 26) % 26 + 'A';
      j = (j + 1) % key.length();
    }

    return decrypted;
  }

  void brutishDecrypt(std::string ciphertext, int keylength, int firstwordlength){
    std::vector<std::string> list;
    std::clock_t start;
    double duration;


    // Initialize key with a's
    char arr[keylength];
    char* keyArr = arr;

    std::string plaintext = "";
    std::string substring;

    setKey(std::string(keyArr));

    for(int i = 0; i < keylength; i++){
      keyArr[i] = 'A';
    }

    setKey(std::string(keyArr, keylength));

    int i = 0;
    start = std::clock();
    while(!isZs(keyArr, keylength)){

      plaintext = decrypt(ciphertext.substr(0, firstwordlength));

      auto it = m.find(plaintext);
      if (it != m.end()){
        std::cout << "Found string " << plaintext << " at " << it->second << " with key " << key << "\n";
        possiblekeys.push(key);
      }

      char* newKey = keyInc(keyArr, keylength);
      setKey(std::string(newKey, keylength));
    }


    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    while(!possiblekeys.empty()){
      std::cout << "Completed with keylength " << key.length() << " in " << duration << ".\n";
      setKey(possiblekeys.front());
      plaintext = decrypt(ciphertext);
      std::cout << "Plaintext with key " << key << " is " << plaintext << "\n";
      possiblekeys.pop();
    }
    std::cout << std::endl;

  }

};

int main(){

  std::cout << "\nStarting bruteforce...\n\n";
  VigenereCipher engine = VigenereCipher("dict.txt"); 

  //engine.brutishDecrypt(ciphertext, keylength, firstwordlength);
  engine.brutishDecrypt("MSOKKJCOSXOEEKDTOSLGFWCMCHSUSGX", 2, 6); // KS
  engine.brutishDecrypt("OOPCULNWFRCFQAQJGPNARMEYUODYOUNRGWORQEPVARCEPBBSCEQYEARAJUYGWWYACYWBPRNEJBMDTEAEYCCFJNENSGWAQRTSJTGXNRQRMDGFEEPHSJRGFCFMACCB", 3, 7);
  engine.brutishDecrypt("MTZHZEOQKASVBDOWMWMKMNYIIHVWPEXJA", 4, 10);
  engine.brutishDecrypt("HUETNMIXVTMQWZTQMMZUNZXNSSBLNSJVSJQDLKR", 5, 11);
  engine.brutishDecrypt("LDWMEKPOPSWNOAVBIDHIPCEWAETYRVOAUPSINOVDIEDHCDSELHCCPVHRPOHZUSERSFS", 6, 9);
  engine.brutishDecrypt("VVVLZWWPBWHZDKBTXLDCGOTGTGRWAQWZSDHEMXLBELUMO", 7, 13);



}
