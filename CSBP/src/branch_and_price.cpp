// 2023-03-15

#include "CSBP.h"
using namespace std;

int BranchAndPriceTree(All_Values& Values, All_Lists& Lists)
{
	Values.node_num = 1; // Root Node already generated

	while (1)
	{
		if (Values.search_flag == 0) // search_flag set to branch current Parent Node
		{		
			Node parent_node;
			InitParentNode(Values, Lists, parent_node); // decide the Node to branch

			Node new_left_node;
			Node new_right_node;

			// always start from the Left Node
			Values.branch_status = 1; 
			Values.node_num++;
			GenerateNewNode(Values, Lists, new_left_node,parent_node); // set the Left Node
			NewNodeColumnGeneration(Values, Lists, new_left_node, parent_node); // solve the Left Node with CG loop
			int left_search_flag = BranchOrSearch(Values, Lists, new_left_node); // finish the Left Node

			// Then the Right Node
			Values.branch_status = 2;
			Values.node_num++;
			GenerateNewNode(Values, Lists, new_right_node,parent_node);  // set the Right Node
			NewNodeColumnGeneration(Values, Lists, new_right_node, parent_node); // solve the Right Node with CG loop
			int right_search_flag = BranchOrSearch(Values, Lists, new_right_node);  // finish the RightNode

			// the var-to-branch val of the Parent Node decide which Node to fathom in next while-iter
			double parent_branch_val = parent_node.var_to_branch_soln_val;
			if(parent_branch_val > 1) 
			{
				if (new_left_node.lower_bound < new_right_node.lower_bound) // choose the Node with better LB to fathom
				{
					Values.search_flag = left_search_flag; 

					if (Values.search_flag != 1)
					{
						Values.fathom_flag = 1; //  fathom_flag set to fathom the Left Node and branch it in next while-iter

						printf("\n	LLB %.4f < RLB %.4f, continue to fathom LEFT Node_%d\n",
							new_left_node.lower_bound, new_right_node.lower_bound, new_left_node.idx);
					}
				}
				else
				{
					Values.search_flag = right_search_flag; 

					if (Values.search_flag != 1)
					{
						Values.fathom_flag = 2; // fathom_flag set to fathom the Right Node and branch it in next while-iter

						printf("\n	Left Node_%d LB %.4f >= Right Node_%d LB %.4f \n\n	continue to fathom RIGHT Node_%d\n",
							new_left_node.idx, new_left_node.lower_bound, 
							new_right_node.idx, new_right_node.lower_bound,
							new_right_node.idx);
					}					
				}
			}
			else // if 
			{
				Values.search_flag = right_search_flag; 

				if (Values.search_flag != 1)
				{
					Values.fathom_flag = 2; // fathom_flag set to fathom the Right Nodeand branch it in next while - iter

					printf("\n	parent branch val = %.4f < 1, \n\n	Have to fathom Right Node_%d",
						parent_branch_val, new_right_node.idx);
				}
			}

			Values.branch_status = 1;  // branch_status set to the Left Node in next while-iter
		}

		if (Values.search_flag == 1) // search_flag set to find a previously generated Node
		{
			Values.branch_status = 3; // branch_status set to find a previously generated unbranched unpruned Node in Tree

			Values.fathom_flag = -1; // better deactivate fathom_flag

			Values.search_flag = 0; // search_flag set to continue to the next while-iter

			printf("\n	Solns of this Node are all INTEGERS! \n");

			break;
		}

		if (Values.node_num > 100)
		{
			printf("\n	//////////// PROCEDURE STOP 3 //////////////\n");
			break;
		}

		// continue while-iter
	}

	return 0;
}