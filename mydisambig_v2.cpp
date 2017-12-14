#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <vector>
#include "Ngram.h"
#include "VocabMap.h"
#include <typeinfo>
#include <map>
using namespace std;
//void show_2D_v(vector<vector<string> > input);
void show_v(vector<string> input);

static Vocab voc;

const VocabIndex emptyContext[] = {Vocab_None};
/*
void viterbi(Ngram& lm, char *map_name, VocabString* input_words, unsigned length){
    char* map_line = '\0';
    int index;
    vector<vector<long double> > delta;//[i][t]
    vector<vector<double> > b;
    vector<vector<double> > a;
    vector<double> pi;
    VocabString output_words[maxWordsPerLine];
    
    
    output_words[0] = input_words[0];//SoS
    VocabIndex SoS[] = {voc.getIndex("<s>"), Vocab_None};
    for(int i = 1; i < length; i++){
    	VocabIndex wid = voc.getIndex(input_words[i]);
    	
    	if(wid != Vocab_None){
    		//cout<<"fuck ";
    		output_words[i] = input_words[i];
    		if(i == 1){
    			pi.push_back(lm.wordProb(wid, SoS));
    		}
    		else{

    		}
    	}
    	else{
    		//map<VocabString VocabString> mapped_words = search_map(map_name, input_words, i);
    		char* a = '\0';
    		cout<<"---------------ha";
    		cout<<" "<<typeid(input_words).name();
    		
    		cout<<typeid(*mapped_words).name();
    		cout<<" ";
    		cout<<typeid(input_words[0]).name();
    		//cout<<" map "<<*mapped_words[0]<<" map ";
    		
    		output_words[i] = input_words[i];
    	}
    }
    
    for(int i = 0; i < length; i++){
    	cout << output_words[i];
    }    
    cout<<endl;
}
*/

int main(int argc, char* argv[]){
    int order = atoi(argv[8]);
    //char* map_name[19];
    
    Ngram lm(voc, order);
    /*
    text argv[2], map argv[4]
    lm argv[6], order argv[8]
    Everything printed will be in result2/xx.txt
    */

    

    File lm_file(argv[6], "r");
    lm.read(lm_file);
    lm_file.close();

	//File map_file(argv[4], "r");
	/*
    string map_name(argv[4]);
    read_write_file rw_map(map_name);
    vector<string> map_file = rw_map.read();
    map<string, vector<string> > zhu_yin_map;
*/

    ifstream map_file(argv[4]);
    string map_line;
    map<string, vector<string> > zhu_yin_map;
    //create zhu yin dictionary---------------
    while (getline(map_file, map_line)) {
        istringstream ss(map_line);
        string zhu, word;
        ss >> zhu;
        while (ss >> word){
        	zhu_yin_map[zhu].push_back(word); 
        }
    }
    zhu_yin_map["<s>"] = {"<s>"};
    zhu_yin_map["</s>"] = {"</s>"};
    //---------------------------------------
    map_file.close();
    ifstream testdata(argv[2]);
    string line;
    while(getline(testdata, line)){
    	istringstream ss(line);
    	while(ss >> next){
    		if(zhu_yin_map[next] == 1){//not a zhu yin

    		}
    		else{
    			
    		}
    	}
    }
    
    
    

    
    testdata.close();

	return 0;
}

void show_v(vector<string> input){
    for(int i = 0;i < input.size();i++){
        cout<<input[i]<<endl;
    }
}/*
void show_2D_v(vector<vector<string> > input){
    for(int i = 0; i < input.size(); i++){
        cout <<endl<<"[ ";
        for(int j = 0; j < input[0].size(); j++){
            cout << input[i][j]<<" ";
            if (input[i][j] > 1){
                //cout <<endl<<" Probability > 1 !"<<endl;
            }
        }
        cout << " ]"<<endl;
    }
}*/
