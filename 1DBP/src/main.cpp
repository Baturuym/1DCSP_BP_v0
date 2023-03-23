﻿// 2022-11-17 branch and price
// 2022-11-23 Branch and Price
// 2023-02-23 

// CG -- column generation
// MP -- master  problem
// SP  -- sub problem
// LB  -- lower bound
// UB -- upper bound

#include "CSBP.h"
using namespace std;

int main()
{
	clock_t start, finish;
	start = clock();

	All_Lists Lists;
	All_Values Values;

	tuple<int, int, int> fileTxt;
	fileTxt = ReadData(Values, Lists);

	Values.stocks_num = get<0>(fileTxt);// number of all stocks
	Values.item_types_num = Lists.all_item_types_list.size(); // number of item types
	Values.stock_length = get<2>(fileTxt); // item_type_length of a stock

	Node root_node; // Init Root Node
	root_node.index = 1; // Node index
	Values.branch_status = 0;

	PrimalHeuristic(Values, Lists, root_node); // generate Root Node matrix
	RootNodeColumnGeneration(Values, Lists, root_node);
	Values.search_flag = FinishNode(Values, Lists, root_node); // find the branch var of Root Node
	Lists.all_nodes_list.push_back(root_node);
	Values.root_flag = 1;

	printf("\n	Current Optimal Lower Bound = %f\n", Values.tree_optimal_lower_bound);

	// continue to BP
	if (Values.search_flag == 0)
	{
		Values.branch_status = 1;
		BranchAndPriceTree(Values, Lists); // Branch and Price loop
	}

	finish = clock();
	double duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("\n	Process Time = %f seconds\n", duration);

	Lists.all_nodes_list.clear();

	cout << endl;
	return 0;
}

