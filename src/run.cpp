#include <iostream>
#include <stdlib.h>

#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/core/p_queue.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/system/timer.h>
#include <LEDA/graph/min_span.h>
#include <LEDA/core/dynamic_trees.h>
#include <assert.h>
#include <LEDA/core/d_array.h>

using namespace std;

// Header file MST.hpp contains the class and its functions of the Minimum Spanning tree
#include "MST.hpp"

int main(){


//-- Test Case #1
// Cases with random graphs
cout<<"\n\n";
cout<<"/------------------------------------\\"<<endl;
cout<<"|----------- Test case #1 -----------|"<<endl;
cout<<"|------------------------------------|"<<endl;
cout<<"| # "<<" MST  "<<"     LEDA      Check       |"<<endl;
cout<<"|------------------------------------|"<<endl;

int n[] = {1000,4000,8000};
for(int i=0; i<3; i++){
		
	leda::graph G;
	int m = n[i]*log10(n[i]);
	
	leda::random_graph(G,n[i],m,true,true,true);
	
	// Construct and check Minimum Spanning Tree with our functions
	// based on the Kruskal's algorithm
	MST *min_span_tree = new MST(G);
	
	min_span_tree->build_MST();
	min_span_tree->checker();

	leda::timer l;

	// LEDA's function for mst
	l.reset(); l.start();
	leda::MIN_SPANNING_TREE(G,min_span_tree->edge_costs);
	l.stop();
	cout<<"| "<<i+1<<": "<<min_span_tree->t<<"      "<<l<<"     checked      |"<<endl;	

	delete min_span_tree;
}
cout<<"\\------------------------------------/"<<endl;


//-- Test Case #2
// Cases with grid graphs
cout<<"\n\n";
cout<<"/------------------------------------\\"<<endl;
cout<<"|----------- Test case #2 -----------|"<<endl;
cout<<"|------------------------------------|"<<endl;
cout<<"| # "<<" MST  "<<"     LEDA      Check       |"<<endl;
cout<<"|------------------------------------|"<<endl;


for(int i=1; i<4; i++){
	leda::graph G;
	leda::grid_graph(G,i*30);
	
	// Construct and check Minimum Spanning Tree with our functions
	// based on the Kruskal's algorithm
	MST *min_span_tree = new MST(G);

        min_span_tree->build_MST();
	min_span_tree->checker();

	leda::timer l;
        l.reset(); 
        l.start();
        
	// LEDA's function for mst
	leda::MIN_SPANNING_TREE(G,min_span_tree->edge_costs);
        l.stop();
        cout<<"| "<<i+1<<": "<<min_span_tree->t<<"      "<<l<<"     checked      |"<<endl;

        delete min_span_tree;
}
cout<<"\\------------------------------------/"<<endl;

//-- Test Case #3
// Cases with random graphs testing the worst case of the algorithm
cout<<"\n\n";
cout<<"/------------------------------------\\"<<endl;
cout<<"|----------- Test case #3 -----------|"<<endl;
cout<<"|------------------------------------|"<<endl;
cout<<"| # "<<" MST  "<<"     LEDA      Check       |"<<endl;
cout<<"|------------------------------------|"<<endl;



int nw[] = {2000,5000,10000};
for(int i=0; i<3; i++){

	int m = nw[i]*log10(nw[i]);

	leda::graph G;
	
	// First construct graph leading to the worst case for Kruskal's algorithm
	MST *min_span_tree = new MST(G);
	min_span_tree->build_MST_worst_case(nw[i],m);

	// Construct and check MST
	min_span_tree->build_MST();
	min_span_tree->checker();

	leda::timer l;
        l.reset();
        l.start();

	// LEDA's function for mst
        leda::MIN_SPANNING_TREE(G,min_span_tree->edge_costs);
        l.stop();
	cout<<"| "<<i+1<<": "<<min_span_tree->t<<"      "<<l<<"     checked      |"<<endl;

	
	delete min_span_tree;	

}
cout<<"\\------------------------------------/"<<endl;
cout<<"\n\n";


}

