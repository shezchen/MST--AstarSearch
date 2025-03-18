//
// Created by aoyeningluosi on 25-3-18.
//
#include <algorithm>

#include "header.h"

AstarSearch::AstarSearch(TSProblem problem):problem(problem)
{
    int current = problem.startNodeIndex;
    //current是旅行商的起始位置
    int lowerBound = problem.graph.getMST().totalWeight;
    std::shared_ptr<AstarNode> ptr = std::make_shared<AstarNode>(std::vector<int>(1,current), 0, current, lowerBound);
    AstarNodes.push_back(ptr);
    
    openList.push_back(AstarNodes.front());//第一次构造使得openList中只有一个元素
    
}

void AstarSearch::expandNode(std::shared_ptr<AstarNode> node)
{
    //首先在开放列表中去除这个node
    openList.erase(std::remove(openList.begin(), openList.end(), node), openList.end());
    
    auto allAvailableCitiesEdges = problem.graph.nodes[node->currentCityIndex].edges;

    
    allAvailableCitiesEdges.erase(std::ranges::remove_if(allAvailableCitiesEdges,
            [node](std::pair<int, int> edge)
                 {
                     return std::find(node->graphNodesVisitedIndex.begin(),
                         node->graphNodesVisitedIndex.end(), edge.first) != node->graphNodesVisitedIndex.end();
                 }).begin(), allAvailableCitiesEdges.end());
    //删除已经访问过的城市的边

    std::sort(allAvailableCitiesEdges.begin(), allAvailableCitiesEdges.end(),
        [node](std::pair<int, int> edge1, std::pair<int, int> edge2)->bool
        {
            return std::get<1>(edge1) < std::get<1>(edge2);
        });
    //按照边的权重排序

    for (auto edge : allAvailableCitiesEdges)
    {
        //为了在AstarNodes中构造新的结点，需要构造新的Nodes

        //构造graphNodesVisitedIndex
        std::vector<int> newGraphNodesVisitedIndex = node->graphNodesVisitedIndex;
        newGraphNodesVisitedIndex.push_back(node->currentCityIndex);

        //构造pathLength
        int newPathLength = node->pathLength + std::get<1>(edge);

        //构造currentCityIndex
        int newCurrentCityIndex = std::get<0>(edge);

        //构造lowerBound
        MST mst = problem.graph.getMST();
        int newLowerBound = mst.totalWeight;
        std::shared_ptr<AstarNode> ptr = std::make_shared<AstarNode>(newGraphNodesVisitedIndex, newPathLength, newCurrentCityIndex, newLowerBound);
        AstarNodes.emplace_back(ptr);
        openList.push_back(AstarNodes.back());

        if (newGraphNodesVisitedIndex.size() == problem.graph.nodes.size())
        {
            //如果所有的城市都已经访问过了，那么就不再继续扩展
            searchIsSolved = true;
            finalNode = AstarNodes.back();
            return;
        }
    }
    
    
}

AstarNode::AstarNode(std::vector<int> graphNodesVisitedIndex, int pathLength,
    int currentCityIndex, int lowerBound)
    :graphNodesVisitedIndex(graphNodesVisitedIndex),
pathLength(pathLength),currentCityIndex(currentCityIndex),lowerBound(lowerBound)
{
    history=graphNodesVisitedIndex;
    history.push_back(currentCityIndex);
    allCost=pathLength+lowerBound;
}

void AstarSearch::sortOpenList()
{
    std::sort(openList.begin(), openList.end(),
        [](std::shared_ptr<AstarNode> a, std::shared_ptr<AstarNode> b)
        {
            return a->allCost < b->allCost;
        });
}
