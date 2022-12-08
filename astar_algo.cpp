// Author : Urvil Nileshbhai Jivani
// ID : 2017A7PS0943G

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>
#include<bits/stdc++.h>
using namespace std;
 #define pb push_back
#define pf pop_front
int tot_nodes_expanded ;
#define State string 
int max_fringe_size ;
map<State,int>smallestDistFromRoot;

int getInvCount(State s)
{ 
    int inv_count = 0; 
    for (int i = 0; i < 8; i++) 
        for (int j = i+1; j < 9; j++) 
             // Value 0 is used for empty space 
             if (s[j]!='0' && s[i]!='0' &&  s[i] > s[j]) 
                  inv_count++; 
    return inv_count; 
} 
  
// This function returns true if given 8 puzzle is solvable. 
bool isSolvable(State s) 
{ 
    // Count inversions in given 8 puzzle 
    int invCount = getInvCount(s); 
  	//cout<<"Inv count is :" <<invCount<<endl;
    // return true if inversion count is even. 
    return (invCount%2 == 0); 
}

int max(int x , int y)
{
	if(x>y)
		return x ;
	return y ;
}
int heuristic_1(State s) // manhattan
{
	int ans = 0 ;
	int orgX , orgY , currX ,currY ;
	for(int i = 0 ; i<9 ; i++)
	{
		orgX = i/3 ; 
		orgY = i%3 ;
		currX = (s[i]-'0')/3 ;
		currY = (s[i]-'0')%3 ;
		ans += abs(orgX-currX)+abs(orgY-currY) ; 
	}
	return ans ;
}
int heuristic_2(State s) // mismatch
{
	int ans = 0 ;
	for(int i = 0 ; i<9 ; i++ )
	{
		ans += (i == (s[i]-'0')) ;
	}
	return ans ;
}

int heuristic_func(State s, int type){
	if(type == 1)return heuristic_1(s);
	else if(type == 2)return heuristic_2(s);
	else return 0;
}

void printstate(State s){
	for(int i=0 ; i<3 ; i++){
		for(int j=0 ; j<3 ; j++){
			cout << s[i*3+j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}
State givenewstate(){
	State newstate;
	cout << "Enter new State:(in the form of a string )\n";
	cin>>newstate ;
	return newstate;
}
 
State generaterandomstate(){
	State newstate;
	newstate = "012345678" ;
	random_shuffle(newstate.begin(),newstate.end());
	return newstate;
}

vector<State>ExpandIntoNeighbours(State curState){
    		int zerorow, zerocol;
    		State temp = curState ;
			for(int i=0 ; i<3 ; i++){
				for(int j=0 ; j<3 ; j++){
					if(temp[i*3+j] == '0'){zerorow = i; zerocol = j; break;}
				}
			}
			
			vector<State> neighbour ;

			// If left is required
			swap(temp[zerorow*3+zerocol], temp[zerorow*3+zerocol+1]);
			if(zerocol < 2 ){
				neighbour.push_back(temp);
			}
			swap(temp[zerorow*3+zerocol], temp[zerorow*3+zerocol+1]);
			// If right is required
			swap(temp[zerorow*3+zerocol], temp[zerorow*3+zerocol-1]);
			if(zerocol >= 1 ){
				neighbour.push_back(temp);
			}
			swap(temp[zerorow*3+zerocol], temp[zerorow*3+zerocol-1]);
			// If up is required
			swap(temp[zerorow*3+zerocol], temp[(zerorow+1)*3+zerocol]);
			if(zerorow < 2 ){
				neighbour.push_back(temp);
			}
			swap(temp[zerorow*3+zerocol], temp[(zerorow+1)*3+zerocol]);
			// If down is required
			swap(temp[zerorow*3+zerocol], temp[(zerorow-1)*3+zerocol]);
			if(zerorow >= 1  ){
				neighbour.push_back(temp);
			}
			swap(temp[zerorow*3+zerocol], temp[(zerorow-1)*3+zerocol]);
			return neighbour;
}


vector<State>Expand(State s, int depth){
    deque<State>que;
    que.pb(s);
    map<State, int>stateDepth;
    stateDepth[s] = depth;
    vector<State>ans;
    while(!que.empty()){
        State curState = que[0];
        ans.pb(curState);
        vector<State>neighbours = ExpandIntoNeighbours(curState);
        for(auto neighbour : neighbours){
            // If already calculated State Depth, ignore
            if(stateDepth.find(neighbour) != stateDepth.end()){continue;}
            // If depth of the neighbour is 0, no need to add it in the return que
            if(stateDepth[curState] == 0){continue;}
            stateDepth[neighbour] = stateDepth[curState] - 1;
            if(smallestDistFromRoot.find(neighbour) == smallestDistFromRoot.end()){
                smallestDistFromRoot[neighbour] = smallestDistFromRoot[curState] + 1;   
            }
            que.push_back(neighbour);
        }
        que.pf();
    }
    return ans;
}

int AStar(State start, State end, int depth, int search_type){
    tot_nodes_expanded = 0;
    max_fringe_size = 0 ;
    State curState = start;
    int counter = 0;
    // Maintains the state with least value of g(s) + h(s) in the beginning
    set<pair<int, State> >fringe;
    smallestDistFromRoot.clear();
    smallestDistFromRoot[start] = 0;
    //set<State>openSet;
    map<State,bool>alreadyOccInFringe;
    while(true){
        // If goal state found, return
        if(curState == end){
            tot_nodes_expanded = alreadyOccInFringe.size();
            return smallestDistFromRoot[curState];
        }
        max_fringe_size = max(max_fringe_size,fringe.size());
        vector<State>allStatesUptoDepthD = Expand(curState, depth);
        bool toContinue = false;
        for(auto state : allStatesUptoDepthD){
            int totCost = smallestDistFromRoot[state] + heuristic_func(state,search_type);
            if(state == end){
                tot_nodes_expanded = alreadyOccInFringe.size();   
                return smallestDistFromRoot[state];
            }
            if(fringe.find({totCost,state}) != fringe.end()){continue;}
            if(alreadyOccInFringe.find(state) != alreadyOccInFringe.end()){continue;}
            fringe.insert({totCost,state});
            alreadyOccInFringe[state] = true;
        }
        if(fringe.empty()){break;}
        curState = (*fringe.begin()).second;
        fringe.erase(fringe.begin());
        counter++;
    }
    tot_nodes_expanded = alreadyOccInFringe.size();
    return -1;
}


int main(){
 
	srand (time(NULL));
	State final = "012345678" ;
 
	for(int i=0 ; i<10 ; i++){
		State s1 = generaterandomstate();
		while(!isSolvable(s1))
			s1 = generaterandomstate();
		//s1 = givenewstate() ;
		cout << "Generated the starting state randomly  ------- : \n";
		printstate(s1);
 		cout<<" -------------------------------------  \n"<<endl;
		// Keep a track of runtime and number of operations for each of the four calls
		int oprs[4];
		float runTime[4];
		int fringes[4];		
 		int nodes_expanded[4];
		int maxDepth;
		cout << "Enter the depth of search required: ";
		cin >> maxDepth;
		cout << "******************************************************************************************************************************\n";
 
		// For heuristic function 1
		cout << "Calling A* for Manhatten heur. algo , depth given\n";
		auto start = chrono::steady_clock::now();
		oprs[0] = AStar(s1,final,maxDepth,1); // smallest path
		auto end = chrono::steady_clock::now();
		runTime[0] = (float)chrono::duration_cast<chrono::milliseconds>(end - start).count()/1000.0;
		fringes[0] = max_fringe_size;
		nodes_expanded[0] = tot_nodes_expanded ;
		cout << "******************************************************************************************************************************\n";
 
		// For heuristic function 2
		cout << "Calling A* for Mismatch heur. algo , depth given\n";
		start = chrono::steady_clock::now();
		oprs[1] = AStar(s1,final,maxDepth,2);
		end = chrono::steady_clock::now();
		runTime[1] = (float)chrono::duration_cast<chrono::milliseconds>(end - start).count()/1000.0;
		fringes[1] = max_fringe_size;
		nodes_expanded[1] = tot_nodes_expanded ;
		cout << "******************************************************************************************************************************\n";
 
		// For heuristic function 1 with depth 1
		cout << "Calling A* for Manhatten heur. algo, depth = 1\n";
		start = chrono::steady_clock::now();
		oprs[2] = AStar(s1,final,1,1);
		end = chrono::steady_clock::now();
		runTime[2] = (float)chrono::duration_cast<chrono::milliseconds>(end - start).count()/1000.0;
		fringes[2] = max_fringe_size;
		nodes_expanded[2] = tot_nodes_expanded ;
		cout << "******************************************************************************************************************************\n";
 
		// For heuristic function 1 with max
		cout << "Calling A* for Mismatch heur. algo, depth = 1\n";
		start = chrono::steady_clock::now();
		oprs[3] = AStar(s1,final,1,2);
		end = chrono::steady_clock::now();
		runTime[3] = (float)chrono::duration_cast<chrono::milliseconds>(end - start).count()/1000.0;
		fringes[3] = max_fringe_size;
		nodes_expanded[3] = tot_nodes_expanded ;
		cout << "******************************************************************************************************************************\n";
 
		cout << "Opeartions for Manhatten Heur with given depth: " << oprs[0] << " Time Required: "  <<  runTime[0] << " Fringe Size: " << fringes[0] <<" total nodes expanded :"<< nodes_expanded[0]<< "\n";
		cout << "Opeartions for Mismatch Heur with given depth: " << oprs[1] << " Time Required: "  <<  runTime[1]  << " Fringe Size: " << fringes[1] <<" total nodes expanded :"<< nodes_expanded[1]<< "\n";
		cout << "Opeartions for Manhatten Heur with given depth = 1 : " << oprs[2] << " Time Required: "  <<  runTime[2] << " Fringe Size: " << fringes[2]<<" total nodes expanded :"<< nodes_expanded[2] << "\n";
		cout << "Opeartions for Mismatch Heur with given depth = 1 : " << oprs[3] << " Time Required: "  <<  runTime[3]  << " Fringe Size: " << fringes[3]<<" total nodes expanded :"<< nodes_expanded[3] << "\n";
	}
	return 0;
}