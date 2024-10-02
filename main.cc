#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <sstream>
#include "Bridges.h"
#include "GraphAdjList.h"
using namespace std;

struct Edge {
	string word; //The word we're connected to
	int weight = 0; //How often they follow us
};

struct Vertex {
	string word; //What word this vertex corresponds to, like APPLE
	int count = 0; //How often this word came up
	int total_edge_weight = 0; //Total weight of the edges coming out of this vertex
	int start_count = 0; //How often this word starts a line
	int comma_count = 0; //How often the word has a comma following it
	vector<Edge> edges; //Holds the words we're connected to, and how often they follow us, like {PEAR,3}
};
ostream &operator<<(ostream &outs,const Vertex &v) {
	outs << "Vertex " << v.word << " - Count: " << v.count <<
		" TEW: " << v.total_edge_weight <<
		" Start Count: " << v.start_count <<
		" End Count: " << (v.count - v.total_edge_weight) <<
		" Comma Count: " << v.comma_count << 
		" Edges:" << endl;
	for (const Edge &e : v.edges) {
		outs << '\t' << e.word << ": " << e.weight << endl;
	}
	return outs;
}

//Utility functions for changing words' case
//The graph will hold everything in ALL CAPS

//Turn food -> FOOD
inline void uppercaseify(string &s) {
	for (char &c : s) c = toupper(c);
}
//Turn FOOD -> food
inline void lowercaseify(string &s) {
	for (char &c : s) c = tolower(c);
}
//Turn FOOD -> Food, for starting a sentence
inline void first_cap(string &s) {
	if (!s.size()) return;
	for (char &c : s) c = tolower(c);
	s.at(0) = toupper(s.at(0));
}

//Eliminates all words between square brackets to remove informative lines in lyrics
//Example: "Chorus[GZA]:" becomes "Chorus:"
void strip_brackets(string &s) {
	string retval;
	bool deleting = false;
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] == '[') deleting = true;
		else if (s[i] == ']') deleting = false;
		else if (!deleting) retval.push_back(s[i]);
	}
	s = retval;
}

int main() {
	int maxEdge = 0;
	const string filename;
	cout << "Please enter a text file to open:\n";
	cin >> filename;
	ifstream ins(filename);
	if (!ins) {
		cout << "Bad filename yo\n";
		exit(EXIT_FAILURE);
	}
	
	unordered_map<string,size_t> hash; //Holds the index (in vec) of each word we've read
	vector<Vertex> graph; //Holds each word and who it's connected to
	long long total_start_count = 0; //How many lines of the file we've read

	//Build graph
	string line;
		int test = 0;
	while(getline(ins, line)){
		//FIRST WORDS
		strip_brackets(line);
		stringstream ss(line);
		string start;
		ss >> start;
		Vertex first;
		bool comma = false;
		if(start.back() == ',') comma = true;
		bool sawLetter = false;
		for(int i = 0; i < start.length();){
			if(start[i] == '+' or start[i] == '-' or start[i] == '\'') i++;
			else if(ispunct(start.back())) start.erase(start.size()-1);
			else i++, sawLetter = true;
		}
		if(!sawLetter) continue;
		total_start_count++;

		uppercaseify(start);
		int idx;
		if(hash.find(start) == hash.end()){ 
			hash.emplace(start, graph.size());
			first.word = start;
			//first count
			first.count += 1;
			first.start_count += 1;
			if(comma) first.comma_count++;
			graph.push_back(first);
			idx = graph.size()-1;
		} else {
			idx = hash.find(start)->second;
			//first count
			graph.at(idx).count += 1;
			graph.at(idx).start_count += 1;
			if(comma) graph.at(idx).comma_count++;
		}


		string current;
		//NEXT WORDS
		while(ss >> current){
			Vertex currWord;
			bool comma = false;
			if(current.back() == ',') comma = true;
			bool sawLetter = false;
			for(int i = 0; i < current.length();){
				if(current[i] == '+' or current[i] == '-' or current[i] == '\'') i++;
				else if(ispunct(current.back())) current.erase(current.size()-1);
				else i++, sawLetter = true;
			}
			if(!sawLetter) continue;

			uppercaseify(current);
			if(hash.find(current) == hash.end()){
				hash.emplace(current, graph.size());
				currWord.word = current;
				//second count
				currWord.count += 1;
				if(comma) currWord.comma_count++;
				graph.push_back(currWord);
			} else {
				int idx2 = hash.find(current)->second;
				//second count
				graph.at(idx2).count += 1;
				if(comma) graph.at(idx2).comma_count++;
			}	




		//EDGES
			Edge edge;
			edge.word = current;
			edge.weight++;
			bool foundEdge = false;
			for(Edge& e : graph.at(idx).edges){
				if(e.word == current){
					e.weight += 1;
					if(e.weight > maxEdge) maxEdge = e.weight;
					graph.at(idx).total_edge_weight++;
					foundEdge = true;
				}
			}
			if(!foundEdge){ 
				graph.at(idx).edges.push_back(edge); 
				graph.at(idx).total_edge_weight++;
			}
			idx = hash.find(current)->second;

		}
	}


	//After the graph is loaded, choose what to do
	cout << "1. Print Graph and Quit\n";
	cout << "2. Generate Random Lyrics\n";
	cout << "3. View on BRIDGES\n";
	int choice;
	cin >> choice;
	if (choice == 1) {
		for (const Vertex &v : graph) {
			cout << v;
		}
		cout << "Total Start Count: " << total_start_count << endl;
		exit(0);
	}
	//Generate sentences
	else if (choice == 2) {
		//Sanity check that we read in at least one sentence
		if (!total_start_count) {
			cout << "Read no sentences, bailing out now...\n";
			exit(1);
		}

		int sentences;
		cout << "How many sentences do you wish to make?\n";
		cin >> sentences;
		const int seed;
		cout << "Please enter the random seed:\n";
		cin >> seed;
		srand(seed);

	  	Vertex startingVertex;
		bool secondRun = false;
		bool test = false;
			//First word
		while(sentences){
			int roll = rand() % total_start_count;
			for(Vertex v : graph){
				roll -= v.start_count;
				if(roll < 0){
					startingVertex = v;
					string s = startingVertex.word;
					first_cap(s);
					cout << s;
					break;
				}
			}
			//continue sentence
			while(true){
				int roll2 = rand() % startingVertex.count;
				roll2 -= startingVertex.count - startingVertex.total_edge_weight;
				//end sentence
				if(roll2 < 0){
					cout << '.' << endl;
					sentences--;
					break;
				//get next word
				} else {
					for(Edge e : startingVertex.edges){
						roll2 -= e.weight;
						if(roll2 < 0){
							string s = e.word;
							lowercaseify(s);
							//comma?
							if(startingVertex.comma_count > 0){
								int rollComma = rand() % startingVertex.total_edge_weight + 1;
								if(rollComma <= startingVertex.comma_count){
									cout << ',';
								}
							}
							cout << ' '<< s; 
							startingVertex = graph.at(hash.find(e.word)->second);
							break;
						}
					}
				}
			}
		}
	}
	else {
		//Render the graph on BRIDGES
		//You must fill in the line below with your own info on bridges
		bridges::Bridges bridges (PROJNUM, "USERNAME", "APIKEY");
		bridges.setTitle("Markov");
		bridges.setDescription("Lyrics");
		bridges::GraphAdjList<string> bridgeGraph;

		for(Vertex v : graph){
			bridgeGraph.addVertex(v.word);
			bridgeGraph.getVertex(v.word)->setLabel(v.word + "\nCount: " + to_string(v.count) + "\nTEW: " + to_string(v.total_edge_weight));
		}
		double divide = maxEdge / 10.0;
		if(divide == 0) divide++;
		int max = graph.size() / divide;
		for(Vertex v : graph){
			for(Edge e : v.edges){
				bridgeGraph.addEdge(v.word, e.word);
				double div = e.weight / divide;
				bridgeGraph.getLinkVisualizer(v.word, e.word)->setThickness(div);
				bridgeGraph.getLinkVisualizer(v.word, e.word)->setLabel(to_string(e.weight));
			}
		}

		bridges.setDataStructure(&bridgeGraph);
		bridges.visualize();
	}
}
