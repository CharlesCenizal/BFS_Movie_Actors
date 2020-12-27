
#include <iostream>
#include <stack>
#include <stdexcept>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <list>
using namespace std;


class Node
{
	public:
		string name;
		string common_movie;
		Node * next;
		// default constructor
		Node()
		{
			next = NULL;
		}
		// constructor
		Node(string n)
		{
			name = n;
			common_movie = "none";
			next = NULL;
		}
		Node(string n, string cm)
		{
			name = n;
			common_movie = cm;
			next = NULL;
		}
};
class LinkedList
{
	public:
		Node * truehead; // initialize a Truehead to point to the first time an actor is mentioned
		Node * head;
		int numItems;
		LinkedList()
		{
			truehead = NULL;
			head = NULL;
			numItems = 0;
		}
		LinkedList(string first_actor)
		{
			truehead = new Node(first_actor);
			head = NULL;
			numItems = 0;
		}
		void insert(string n,string cm)
		{
			Node * to_insert = new Node(n,cm);
			to_insert->next = head; //1)  To_insert's next -> head
			head = to_insert; // 2) update head
			truehead->next = to_insert; // 3) update truehead
			numItems ++;
		}

		void print()
		{
			Node * curr = head;
			while(curr != NULL)
			{

				cout << curr->name + " " + curr->common_movie + " ";
				curr = curr->next;
			}
			cout << endl; // ends the line gets rid of the perecent sign at the end
		}

		void printStartingTrueHead()
		{
			Node *curr = truehead;
			while(curr != NULL)
			{
				cout << curr->name + " " + curr->common_movie + " ";
				curr = curr->next;
			}
			cout << endl;
		}

		int list_size()
		{
			return numItems;
		}
};
class Pair
{
public:
	string parent; // parent of v
	string movie; // The common_movie with parent
	Pair()
	{
		parent = "none";
		movie = "none";
	}
};
unordered_map <string,Pair> BFS(string s,unordered_map <string, LinkedList> umap,unordered_map <string, bool> visited)
{
	unordered_map <string,Pair> result;
	Pair to_insert;
	list <string> toVisit; // a queue of stuff to visit
	Node * curr;
	int size;
	visited[s] = true;
	toVisit.push_back(s);

	while( !toVisit.empty())
	{
		s = toVisit.front();
		//cout << s << endl;
		toVisit.pop_front();
		//cout << " gets here " << endl;
		curr = umap[s].truehead;
		size = umap[s].list_size();

		// Get all adjacent verticies of dequeued
		// vertex s


		for(int i = 0; i < size; i ++)
		{
			curr = curr->next;

			if(visited[curr->name] == false) //curr->name is a kiddo
			{

				to_insert.parent = s; // S is the parent (starts as actor 1)
				to_insert.movie = curr->common_movie; // kiddo's common movie with parent
				result[curr->name] = to_insert;
				//cout << curr->common_movie << endl;
				visited[curr->name] = true;
				toVisit.push_back(curr->name);
			}
		}
	}
	return result;
}
stack <class Pair> getAnswer(string actor1, string actor2, unordered_map <string, Pair> pred)
{
	// Base Case: If the the parent of pred[pred[pred[actor2]]]..... is finally actor 1
	stack <class Pair> resultStack;
	Pair to_push;
	string parent_prev;
	string parent = pred[actor2].parent;
	string parent_movie = pred[actor2].movie;
	bool lastRun = false;
	while(true)
	{
		parent_prev = parent; // holds the previous value of parent
		to_push.parent = parent;
		to_push.movie = parent_movie;
		resultStack.push(to_push);
		parent = pred[parent_prev].parent;
		parent_movie = pred[parent_prev].movie;
		if(lastRun)
		{
			break;
		}
		if(parent == actor1)
		{
			lastRun = true;
		}
	}
	return resultStack;
}
int main(int argc, char** argv)
{

	unordered_map <string, LinkedList> umap; // Important! - This is the u-map we are using for the graph

	if(argc != 3)
	{
				printf("Usage: ./bard <INPUT FILE> <OUTPUT FILE>\n");
				exit(EXIT_FAILURE);
	}
	ifstream in; // input file
	ifstream myfile;
	myfile.open("cleaned_movielist.txt");
 	ofstream out; // output file
  in.open(argv[1]);
  out.open(argv[2]);
  string in_str;
	unordered_map  <string, bool> visited_prev; // important
	// Making the graph
	while(getline(myfile,in_str))
	{
		vector <string> current_line; // A vector of actors on a given line
		stringstream line(in_str);
		//cout << in_str << endl;
		string temp_movie;
		string movie; // temp string for movie
		string temp_actor; // temp string for actors


		getline(line,temp_movie,' '); // getting the common_movie
		movie = temp_movie;
		//cout << movie << endl;
		while(getline(line,temp_actor,' '))
		{
			current_line.push_back(temp_actor); // we put the actors in here
		}
		//cout << "Vector size " +to_string(current_line.size()) << endl;
		for(int i = 0; i < current_line.size(); i ++)
		{
			if (umap[current_line[i]].truehead == NULL) // check linkedlists at i
			{
				// current_line[i] is the actor
				umap[current_line[i]] = LinkedList(current_line[i]);
				visited_prev[current_line[i]] = false;
				// If it's null make a new linked lists there
				// with the true head being current_line[i]
			}
			// if linkedlist not NULL, then the actor has been saved before
			// cool. move on.
			for(int j = 0; j < current_line.size(); j++)
			{
				if(j != i)
				{
					umap[current_line[i]].insert(current_line[j],movie);
					// if j != i insert the actor and movie into the LinkedList
					// the linked list should never be NULL because we check beforehand
					// if it's null, we make a new linked list there
				}
			}
		}
	}
	unordered_map  <string, bool> visited = visited_prev; // Important don't comment out
	// The graph has been made :)
	// Testing the graph
	// testing on simple.txt
	/*
	umap["Rohan"].printStartingTrueHead();
	umap["Charles"].printStartingTrueHead();
	umap["Rahil"].printStartingTrueHead();
	umap["David"].printStartingTrueHead();

	cout << "size of rahil is " + to_string(umap["Rahil"].list_size()) << endl;
	cout << "size of map is " + to_string(umap.size()) << endl;
	cout << "size of visited_prev is " + to_string(visited_prev.size()) << endl;
	*/
	// Graph works and Graph is fast :)

	// Implement Breadth First Search
	// work on getting input from input File


	/*
	cout << "size of visited is " + to_string(visited.size()) << endl;

	int listSize = umap["Rahil"].list_size();
	Node * curr = umap["Rahil"].truehead;

	for(int i = 0; i < listSize; i ++)
	{
		curr = curr->next;
		if(i == listSize-1)
		{
			cout << "Make it or break it" << endl;
		}
		cout << "pls work: "  + curr->name << endl;

	}
	*/
	int size_of_map = umap.size();

	while(getline(in,in_str))
	{
		bool fail_at_start = false;
		string actor1;
		string actor2;
		string actor_temp;

		stringstream line(in_str);

		getline(line,actor_temp,' '); // get the first actor
		actor1 = actor_temp;

		getline(line,actor_temp,' ');
		actor2 = actor_temp;
		// getting the actors works
		//cout << actor1 + " " + actor2 << endl;

		// Edge Cases :)
		// If EITHER (OR) actor1 or actor2 are not present
		// print "Not present"
		if(umap[actor1].truehead == NULL || umap[actor2].truehead == NULL)
		{
			out << "Not present" << endl;
		}
		// If actor1 is the same as actor2 just print actor 1
		else if(actor1 == actor2)
		{
			out << actor1 << endl;
		}
		else // DO BFS! :)
		{
			// Implement BFS
			unordered_map < string, Pair > pred;
			pred = BFS(actor1,umap,visited);
			//out << "BFS" << endl;
			//cout << pred[actor2].parent << endl;
			//cout << pred[actor2].movie << endl;

			//cout << endl;

			//cout << pred[pred[actor2].parent].parent << endl;
			//cout << pred[pred[actor2].parent].movie << endl;

			//cout << endl;

			//cout << pred[pred[pred[actor2].parent].parent].parent << endl;
			//cout << pred[pred[pred[actor2].parent].parent].movie << endl;

			stack <class Pair> resultStack;
			resultStack = getAnswer(actor1,actor2,pred);
			Pair stacktop;
			while(!resultStack.empty())
			{
				stacktop = resultStack.top();
				out <<   stacktop.parent;
				out << " -(" +stacktop.movie + ")- ";
				resultStack.pop();
			}
			out << actor2 << endl;

			visited = visited_prev; // reset visited to it's previous state
			//BFS(actor1,umap,visited);



		}

	}

	return(EXIT_SUCCESS);

}
