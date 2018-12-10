
// Class MST contains the functions and some variables of the Minimum Spanning Tree
class MST{
public:
	void build_MST();				// Build Minimum Spanning Tree based on the algorithm of Kruskal
	void checker();					// Function illustrates a checker program for the minimum spanning tree
	void build_MST_worst_case(int m, int n);	// Build a graph that illustrates the worst case for the algorth above
	leda::timer t;					// Variable for measuring the construction time
	leda::edge_array<int> edge_costs;		// Array containing the costs of the edges
	leda::d_array<leda::edge,int> worst_case_costs;
	MST(leda::graph G):GR(G){};			// Constructor
	
private:
	leda::list<leda::edge> MST_edges, MST_edges_out;// Edges included in the MST and not
	leda::graph GR;					// Initial graph
};

void MST::build_MST(){
	
	
	// Priority queue containing the edges of the graph
	leda::p_queue<int, leda::edge> P;
	leda::edge e;
	leda::node u,v;
	edge_costs = GR;

	// Assign random costs to the edges of the graph, or costs for the worst case
	if(worst_case_costs.empty()){

		forall_edges(e,GR){ edge_costs[e] = rand() % 10001; P.insert(edge_costs[e],e);}
	}else{
		forall_edges(e,GR){ edge_costs[e] = worst_case_costs[e]; P.insert(edge_costs[e],e);}
	}
	t.reset();	
	t.start();

	// Node map to check for cycles in the graph
	leda::node_map<leda::list<leda::node>*> M;
	M = GR;

	// Take the edge with the minimum cost
	while(!P.empty()){
		e = P.inf(P.find_min());
		u = GR.source(e);
		v = GR.target(e);

		// check where to put its nodes in the node map and check for cycles
		if(M[u]==NULL && M[v]==NULL){
			leda::list<leda::node> *m = new leda::list<leda::node>;
			M[u] = m;
			M[v] = m;
			M[u]->append(u);
			M[v]->append(v);
			MST_edges.push(e);
			M[u] = m;
			delete m; 
		
		}else if(M[u]==NULL && M[v]!=NULL){
			M[v]->append(u);
			M[u] = M[v];
			MST_edges.push(e);
		}else if(M[u]!=NULL && M[v]==NULL){
			M[u]->append(v);
			M[v] = M[u];
                        MST_edges.push(e);
		}else{
			if (M[u]==M[v]){
				MST_edges_out.push(e);
			}else{
				if( M[u]->length() < M[v]->length()){
					leda::node k;
					forall(k,*M[u]){M[k] = M[v];}
					M[v]->conc(*M[u],leda::behind);
					MST_edges.push(e);
				}else{
					leda::node k;
                                        forall(k,*M[v]){M[k] = M[u];}
					M[u]->conc(*M[v],leda::behind);
					MST_edges.push(e);
				}
			}
		}
		// Delete the edge and repeat
		P.del_min();
		
	}

	t.stop();
}

void MST::checker(){
	
	leda::dynamic_trees T;
	leda::node_map<leda::vertex> V;
	V = GR;

	leda::node nd,u,v;
	forall_nodes(nd,GR){V[nd] = T.make();}
		
	leda::edge e;

	// Create links for the nodes of the MST
	while(!MST_edges.empty()){
		e = MST_edges.pop();
		u = GR.source(e);
		v = GR.target(e);
		
		T.evert(V[u]);
		T.evert(V[v]);		
		T.link(V[u],V[v],edge_costs[e]);
	}
	
	// Check all edges that are not in the Minimum spanning tree, if
	// the costs is smaller by including them
	while(!MST_edges_out.empty()){
		e = MST_edges_out.pop();
		u = GR.source(e);
                v = GR.target(e);
		
		leda::vertex first = V[u];
		leda::vertex second = V[v];
		leda::vertex ancestor = T.lca(first,second);
		
		// If it is the program will stop			
		while(first != ancestor){
			assert(edge_costs[e] >= T.cost(first));
			first = T.parent(first);
		}
		while(second != ancestor){
			assert(edge_costs[e] >= T.cost(second));
			second = T.parent(second);
		}
	}

}

void MST::build_MST_worst_case(int n, int m){
		
	int levels = round(log(n-1)/log(2));
		
	int count, c; bool flag;
	count=1; c=2; flag = true;

	for(int i=0; i<n; i++){ GR.new_node();}
	leda::node p = GR.first_node();
	
	// Create edges with costs in order to achive the most merge lists
	// in the node map while constructing the MST
	while(p!=GR.last_node()){
		leda::node next = GR.succ_node(p);
		leda::edge e = GR.new_edge(p,next);

		if(count%2==1){worst_case_costs[e]=1;}
		else{
			worst_case_costs[e] = c;
			if(flag){
				c++;
				if(c==levels+1){
					flag=false;
					c = c-2;
				}
			}else{
				c--;
				if(c==1){
					flag=true;
					c++;
				}
			}
		}
		count++;
		p = GR.succ_node(p);

	}

	// Give random costs to the rest of the edges of the graph
	int rest = m - n +1;
	while(rest!=0){
		leda::node u = GR.choose_node();
		leda::node v = GR.choose_node();

		while(u==v){  u = GR.choose_node();  v = GR.choose_node(); }
		
		leda::edge e = GR.new_edge(u,v);
		worst_case_costs[e] = levels + (rand() % 10001);
		rest--;
	}
		
	leda::Make_Connected(GR);
	
}
