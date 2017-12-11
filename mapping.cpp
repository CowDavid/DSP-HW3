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
vector<string> make_initials(vector<string> input);
vector<string> mapping_with_initial(string initial, vector<string> input, vector<string> output);
void show_v(vector<string> input);
string check_print = "I love Misaka Mikoto<3";
int main(int argc, char* argv[]){
    cout << "Reading " << argv[1] << "..." <<endl;
	read_write_file rw_input(argv[1]);
    read_write_file rw_output(argv[2]);
	vector<string> input = rw_input.read();
    vector<string> output;//37 ZhuYin
    char character[4];
    read_write_file rw_initials("ZhuYinInitials.txt");
    vector<string > initials = make_initials(rw_initials.read());  
    for(int i = 0; i < initials.size(); i++){
        output = mapping_with_initial(initials[i], input, output);
    }
    cout << "Writing " << argv[2] << "..." <<endl;
    rw_output.write(output);
	return 0;
}
vector<string> make_initials(vector<string> input){
    string initial_help;
    vector<string > initials;
    for(int i = 0; i < 37; i++){
        initial_help.assign(input[i], 0, 2);
        initials.push_back(initial_help);
        initial_help = '\0';
    }
    return initials;
}
vector<string> mapping_with_initial(string initial, vector<string> input, vector<string> output){
    //ㄅ      八 匕 卜 不 卞 巴 比 丙 包 ...
    string line;
    string help;
    string help_add;
    string space;
    string longspace;
    line = initial + "     ";
    vector<string> lines_left;
    bool flag = true;
    for(int i = 0; i < input.size(); i++){
        help.assign(input[i],3,2);
        if(help == initial){
            help_add.assign(input[i],0,2);
            line = line + " " + help_add;
            lines_left.push_back(help_add + "      " + help_add);
        }
        else{
            for(int j = 0 ; j < input[i].length(); j++){
                if(input[i][j] == '/'){
                    help.assign(input[i],j,2);
                    if(help == initial && flag){
                        help_add.assign(input[i],0,2);
                        line = line + " " +help_add;
                        lines_left.push_back(help_add + "      " + help_add);
                        flag = false;
                    }
                }

            }
            flag = true;
        }
    }
    output.push_back(line);
    for(int i = 0; i < lines_left.size(); i++){
        output.push_back(lines_left[i]);
    }
    return output;
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
