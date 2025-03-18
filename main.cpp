#include <iostream>
#include "header.h"

int main()
{
    int nodesNumber=10;
    std::vector<TSProblem> problems;
    for (int i = 0; i < 10; i++)
    {
        problems.push_back(TSProblem(nodesNumber));

        AstarSearch search(problems[i]);

        int tryTimes=0;
        bool found=true;
        while (!search.searchIsSolved)
        {
            tryTimes++;
            search.sortOpenList();
            search.expandNode(search.openList.front());

            if (tryTimes > 10000)
            {
                found = false;
                break;
            }
        }
        std::cout<<"Problem "<<i+1<<": ";
        for (auto i : search.problem.graph.nodes)
        {
            std::cout<<"Index["<<i.index<<"] ,"<<"Edges:";
            for (auto j:i.edges)
            {
                std::cout<<"["<<j.first<<"]"<<" "<<j.second<<"  ";
            }
            std::cout<<std::endl;
        }
        if (found)
        {
            std::cout<<"Answer: ";
            for (int j = 1; j < search.finalNode->history.size(); j++)
            {
                std::cout<<search.finalNode->history[j]<<" ";
            }
            std::cout<<std::endl<<"Cost: "<<search.finalNode->allCost;
            std::cout<<std::endl<<"----------------------"<<std::endl;
        }else
        {
            std::cout<<"Give up in 10000 times.";
        }
        std::cout<<std::endl;
    }
}