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
#include <vector>
#include <map>
#include <set>
#include <algorithm>

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
  std::set<std::string> wordset;


  void storeDict(std::string dictionaryFile){
    std::ifstream file(dictionaryFile);
    std::string line;

    int i = 1;
    while(std::getline(file, line)){
      wordset.insert(line);
      i++;
    }

    std::set<std::string>::const_iterator it = std::find(wordset.begin(), wordset.end(), "CAESAR");

      if (it != wordset.end()){
           std::cout << "Found '" << *it << "' in the vector." << std::endl;
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
    while(!isZs(keyArr, keylength)){

      plaintext = decrypt(ciphertext.substr(0, firstwordlength));
      //std::cout << plaintext << " " << i << " ";
      
      std::set<std::string>::const_iterator it = std::find(wordset.begin(), wordset.end(), plaintext);

      if (it != wordset.end()){
           std::cout << "Found '" << *it << "' in the vector." << std::endl;
      }
      i++;

      char* newKey = keyInc(keyArr, keylength);
      setKey(std::string(newKey, keylength));
      //std::cout << key << "\n";
    }


  }

};

int main(){

  VigenereCipher engine = VigenereCipher("dict.txt"); 

  //engine.brutishDecrypt(ciphertext, keylength, firstwordlength);
  engine.brutishDecrypt("MSOKKJCOSXOEEKDTOSLGFWCMCHSUSGX", 2, 6); // KT is key (found by visual)

}
