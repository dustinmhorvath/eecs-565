#include <iostream>
#include <string>
#include <fstream>

class VigenereCipher {

public:
  std::string key;

  VigenereCipher(std::string key){
    for(int i = 0; i < key.size(); ++i)
    {
      if(key[i] >= 'A' && key[i] <= 'Z')
        this->key += key[i];
      else if(key[i] >= 'a' && key[i] <= 'z')
        this->key += key[i] + 'A' - 'a';
    }
  }

  std::string encrypt(std::string text){
    std::string out;

    for(int i = 0, j = 0; i < text.length(); ++i){
      char c = text[i];

      if(c >= 'a' && c <= 'z'){
        c += 'A' - 'a';
      }
      else if(c < 'A' || c > 'Z'){
        continue;
      }
      out += (c + key[j] - 2*'A') % 26 + 'A';
      j = (j + 1) % key.length();
    }

    return out;
  }

  std::string decrypt(std::string text){
    std::string out;

    for(int i = 0, j = 0; i < text.length(); ++i)
    {
      char c = text[i];

      if(c >= 'a' && c <= 'z')
        c += 'A' - 'a';
      else if(c < 'A' || c > 'Z')
        continue;

      out += (c - key[j] + 26) % 26 + 'A';
      j = (j + 1) % key.length();
    }

    return out;
  }
};

int main(){
  std::string key = "";
  std::string tempLine;
  std::string input = "";

  std::ifstream keystream ("key.txt");
  if(keystream.is_open()){
    while(keystream.good()){
      getline(keystream,tempLine);
      key = key + tempLine;
    }
    keystream.close();
  }

  VigenereCipher viginere(key);

  std::string line;
  std::ifstream inputstream ("input.txt");
  if (inputstream.is_open())
  {
    while (inputstream.good())
    {
      getline(inputstream,line);
      input = input + line;
    }
    inputstream.close();
  }
  else
  {
    std::cout << "Unable to open file" << std::endl << std::endl;
  }

  std::string encrypted = viginere.encrypt(input);
  std::string decrypted = viginere.decrypt(encrypted);

  std::cout << input << std::endl;
  std::cout << "Encrypted: " << encrypted << std::endl;
  std::cout << "Decrypted: " << decrypted << std::endl;
}
