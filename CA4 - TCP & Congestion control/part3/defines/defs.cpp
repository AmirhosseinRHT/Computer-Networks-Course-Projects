#include "defs.hpp"

using namespace std;

vector<string> splitString(const string &str, char del)
{
    vector<string> result_vector;
    string temp = "";
    for(int i=0; i<(int)str.size(); i++){
        if(str[i] != del){
            temp += str[i];
        }
        else{
            if(temp.size() > 0)
                result_vector.push_back(temp);
            temp = "";
        }
    }
    if(temp.size() > 0)
        result_vector.push_back(temp);
    return result_vector;
}