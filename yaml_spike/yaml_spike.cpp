#include <fstream>
#include "include/yaml.h"

int main()
{
  std::ifstream fin("test.yaml");
  YAML::Parser parser(fin);
  YAML::Node doc;
  std::string name;
  doc["name"] >> name;
  int age;
  doc["age"] >> age;
  std::cout << "Found entry with name '" << name << "' and age '" << age << "'\n";

    return 0;
}

