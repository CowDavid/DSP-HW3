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
void show_2D_v(vector<vector<string> > input);
void show_v(vector<string> input);
long double bigram_prob(const char *previous, const char *current, Ngram *lm);
//void viterbi(string line, map<string, vector<string> > zhu_yin_map, Ngram *lm);
int index_of_max_prob(vector<pair<vector<string>, long double> >word_and_prob);
static Vocab voc;

const VocabIndex emptyContext[] = {Vocab_None};

int main(int argc, char* argv[]){
    int order = atoi(argv[8]);    
    Ngram lm(voc, order);
    /*
    text argv[2], map argv[4]
    lm argv[6], order argv[8]
    Everything printed will be in result2/xx.txt
    */
    File lm_file(argv[6], "r");
    lm.read(lm_file);
    lm_file.close();

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
    //start handling every sentence by viterbi 
    while(getline(testdata, line)){
    	istringstream ss(line);
		string current;
		string previous = "<s>";
		pair<vector<string>, long double> initial({previous}, 0);
		vector<pair<vector<string>, long double> > word_and_prob_p= {initial}; //previous word and its probability
		
		while(ss >> current){
			if(zhu_yin_map[current].size() != 1){//current word is a zhu yin
				vector<pair<vector<string>, long double> > word_and_prob_c;//current word and its probability
				for(auto& i: zhu_yin_map[current]){
					//iteration in the mapped words from current word(a zhuyin)
					vector<pair<vector<string>, long double> > word_and_prob_help; 
					//used to record the probability in every iteration in paths from previous word
					for(int j = 0; j < zhu_yin_map[previous].size(); ++j){
						//iteration in the mapped words from previous word, which also means iteration in every paths from previous word
						long double bi_prob = bigram_prob(zhu_yin_map[previous][j].c_str(), i.c_str(), &lm);
						pair<vector<string>, long double> help(word_and_prob_p[j].first, word_and_prob_p[j].second + bi_prob);
						//add bigram probability to the previous probability
						//p.s. use '+', because it is log(prob)
						word_and_prob_help.push_back(help);//add a path
					}
					pair<vector<string>, double> i_max = word_and_prob_help[index_of_max_prob(word_and_prob_help)];
                    i_max.first.push_back(i);
					word_and_prob_c.push_back(i_max);
				}
				word_and_prob_p = word_and_prob_c;
			}
			else{//current word is not a zhu yin
				
				for (auto& i: word_and_prob_p) {
                    long double bi_prob = bigram_prob(i.first[i.first.size()-1].c_str(), current.c_str(), &lm);
                    i.second += bi_prob;
                }              
                pair<vector<string>, long double> max = word_and_prob_p[index_of_max_prob(word_and_prob_p)];
                for (auto& i: max.first) { cout << i << " "; }
                word_and_prob_p.clear();
				pair<vector<string>, double> guo({zhu_yin_map[current][0]}, 1);
				word_and_prob_p.push_back(guo);
			}
			previous = current;
		}
		
		for (auto& i: word_and_prob_p) {
	            i.second += bigram_prob(i.first[i.first.size()-1].c_str(), "</s>", &lm);
	    }
	    pair<vector<string>, double> max = word_and_prob_p[index_of_max_prob(word_and_prob_p)];
	    for (auto& i: max.first){ 
	    	cout << i << " ";
	    }
	    
	    cout << "</s>" << endl;
    }
    testdata.close();
	return 0;
}
/*
void viterbi(string line, map<string, vector<string> > zhu_yin_map, Ngram *lm){
	istringstream ss(line);
	string current;
	string previous = "<s>";
	pair<vector<string>, long double> initial({previous}, 0);
	vector<pair<vector<string>, long double> > word_and_prob_p= {initial}; //previous word and its probability
	vector<pair<vector<string>, long double> > word_and_prob_c;//current word and its probability
	while(ss >> current){
		if(zhu_yin_map[current].size() != 1){//current word is a zhu yin
			
			
			for(auto& i: zhu_yin_map[current]){
				//iteration in the mapped words from current word(a zhuyin)
				vector<pair<vector<string>, long double> > word_and_prob_help; 
				//used to record the probability in every iteration in paths from previous word
				for(unsigned int j = 0; j < zhu_yin_map[previous].size(); j++){
					//iteration in the mapped words from previous word, which also means iteration in every paths from previous word
					long double bi_prob = bigram_prob(zhu_yin_map[previous][j].c_str(), i.c_str(), lm);
					pair<vector<string>, long double> help(word_and_prob_p[j].first, word_and_prob_p[j].second + bi_prob);
					//add bigram probability to the previous probability
					//p.s. use '+', because it is log(prob)
					word_and_prob_help.push_back(help);//add a path
				}
				word_and_prob_c.push_back(word_and_prob_help[index_of_max_prob(word_and_prob_help)]);
			}
			word_and_prob_p = word_and_prob_c;
		}
		else{//current word is not a zhu yin
			
			vector<pair<vector<string>, long double> > word_and_prob_help; 
			for(unsigned int j = 0; j < word_and_prob_p.size(); j++){
				//iteration in the mapped words from previous word, which also means iteration in every paths from previous word
				long double bi_prob = bigram_prob(zhu_yin_map[previous][j].c_str(), current.c_str(), lm);
				pair<vector<string>, long double> help(word_and_prob_p[j].first, word_and_prob_p[j].second + bi_prob);
				word_and_prob_help.push_back(help);//add a path
			}
			pair<vector<string>, long double> max = word_and_prob_help[index_of_max_prob(word_and_prob_help)];
			word_and_prob_c.push_back(max);
			for (auto& i: max.first) { cout << i << " "; }
			
		}
		previous = current;
	}
	
	for (auto& i: word_and_prob_p) {
            i.second += bigram_prob(i.first[i.first.size()-1].c_str(), "</s>", lm);
    }
    pair<vector<string>, double> max = word_and_prob_p[index_of_max_prob(word_and_prob_p)];
    for (auto& i: max.first){ 
    	cout << i << " ";
    }
    
    cout << "</s>" << endl;
}*/
int index_of_max_prob(vector<pair<vector<string>, long double> >word_and_prob){
	long double max_prob = -10000;
	int index;
	for(int i = 0; i < word_and_prob.size(); i++){
		if(word_and_prob[i].second > max_prob){
			max_prob = word_and_prob[i].second;
			index = i;
		}
	}
	return index;
}

long double bigram_prob(const char *previous, const char *current, Ngram *lm){
	VocabIndex wid_p = voc.getIndex(previous);
	VocabIndex wid_c = voc.getIndex(current);
	//oov handling---
	if(wid_p == Vocab_None) 
        wid_p = voc.getIndex(Vocab_Unknown);
    if(wid_c == Vocab_None) 
        wid_c = voc.getIndex(Vocab_Unknown);
    //---------------
    VocabIndex context[] = { wid_p, Vocab_None};
	long double prob = lm->wordProb(wid_c, context);
	return prob;

}
void show_v(vector<string> input){
    for(int i = 0;i < input.size();i++){
        cout<<input[i]<<endl;
    }
}
void show_2D_v(vector<vector<string> > input){
    for(int i = 0; i < input.size(); i++){
        cout <<endl<<"[ ";
        for(int j = 0; j < input[0].size(); j++){
            cout << input[i][j]<<" ";
        }
        cout << " ]"<<endl;
    }
}
