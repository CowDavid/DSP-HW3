#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include <iomanip>
#include <math.h>
using namespace std;
class read_write_file{
    public:
        read_write_file(string name);
        ~read_write_file();
        vector<string> read();
        void write(vector<string> input);
    private:
        char filename[50];
};

void show_v(vector<string> input);
string check_print = "I love Misaka Mikoto<3";
int main(int argc, char* argv[]){
	read_write_file rw_input(argv[0]);
    read_write_file rw_output(argv[1]);
	vector<string> input = rw_input.read();
    
	//show_v(file);
    cout<<check_print<<endl;
	return 0;
}

//-------read and write class---------
read_write_file::read_write_file(string name){
    for(int i = 0; i < name.length(); i++){
        filename[i] = name[i];
    }
    filename[name.length()]='\0';
}
read_write_file::~read_write_file(){
}
vector<string> read_write_file::read(){
    string line;
    ifstream file (filename);
    vector<string> output_v;
    if (file.is_open()){
        while ( getline (file,line) )
        {
            output_v.push_back(line);
        }
        file.close();
        return output_v;
      }
    else {
        cout << "Unable to open file"<<endl;
        output_v.push_back("Nothing, because I can't open file.");
        return output_v;
    }
}
void read_write_file::write(vector<string> input){
    ofstream file (filename);
    for(unsigned int i = 0;i < input.size();i++){
        file << input[i]<<"\n";
    }
}
//------------------------------------
vector<string> split(string input, char seperator,char seperator2){
    vector<string> output;
    string buf = "";
    int begin_index = 0;
    int count_buf = 0;
    for(unsigned int i = 0; i < input.length(); i++){
        if(input[i] == seperator || input[i] == seperator2){
            for(unsigned int j = begin_index; j < i; j++){
                buf = buf + input[j];
            }
            begin_index = i + 1;
            output.push_back(buf);
            buf = "";
        }
    }
    for(unsigned int j = begin_index; j < input.length(); j++){
        buf = buf + input[j];
    }
    output.push_back(buf);
    return output;
}
void show_v(vector<string> input){
    for(int i = 0;i < input.size();i++){
        cout<<input[i]<<endl;
    }
}
