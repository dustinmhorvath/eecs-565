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

class VigenereCipher {

public:
  std::string key;

  VigenereCipher(std::string key){
    // Force key to uppercase alphanumeric
    for(int i = 0; i < key.size(); ++i){
      if(key[i] >= 'A' && key[i] <= 'Z'){
        this -> key += key[i];
      }
      else if(key[i] >= 'a' && key[i] <= 'z'){
        this -> key += key[i] + 'A' - 'a';
      }
    }
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
};

bool isZs(char key[]){
  bool notzs = true;
  for(int i = 0; i < key.size(); i++){
    if(key[i] != 'Z'){
      notzs = false;
    }
  }
  return notzs;
}

void brutishDecrypt(std::string ciphertext, int keylength, int firstwordlength){
  std::vector<std::string> list;

  // Initialize key with a's
  char key[keylength];
  for(int i = 0; i < keylength; i++){
    key[i] = 'a';
  }

  while(!isZs(key)){

  }


}



int main(){
  std::string key = "";
  std::string tempLine;
  std::string input = "";

  // Read in key file
  std::ifstream keystream ("key.txt");
  if(keystream.is_open()){
    while(keystream.good()){
      // Read lines
      getline(keystream,tempLine);
      key = key + tempLine;
    }
    keystream.close();
  }
  else{
    std::cout << "Unable to open key file" << std::endl << std::endl;
  }

  VigenereCipher viginere(key);

  std::string line;
  // Read in plaintext input
  std::ifstream inputstream ("input.txt");
  if (inputstream.is_open()){
    while (inputstream.good()){
      // Read plaintext lines from file input
      getline(inputstream,line);
      input = input + line;
    }
    inputstream.close();
  }
  else{
    std::cout << "Unable to open plaintext file" << std::endl << std::endl;
  }

  std::string encrypted = viginere.encrypt(input);
  std::string decrypted = viginere.decrypt(encrypted);

  std::cout << "Plaintext: " << input << std::endl;
  std::cout << "Ciphertext: " << encrypted << std::endl;
  std::cout << "Decrypted: " << decrypted << std::endl;
}
