//
// Created by aoyeningluosi on 25-3-18.
//

#include <algorithm>
#include <functional>

#include "header.h"

void Graph::addEdge(int firstNodeIndex,int secondNodeIndex, int weight)
{
    nodes[firstNodeIndex].edges.emplace_back(secondNodeIndex, weight);
    nodes[secondNodeIndex].edges.emplace_back(firstNodeIndex, weight);
}

void Graph::removeEdge(int firstNodeIndex,int secondNodeIndex)
{
    nodes[firstNodeIndex].edges.erase(std::remove_if(nodes[firstNodeIndex].edges.begin(),
        nodes[firstNodeIndex].edges.end(),
        [secondNodeIndex](std::pair<int, int> edge)
        { return edge.first == secondNodeIndex; }), nodes[firstNodeIndex].edges.end());
    nodes[secondNodeIndex].edges.erase(std::remove_if(nodes[secondNodeIndex].edges.begin(),
        nodes[secondNodeIndex].edges.end(),
        [firstNodeIndex](std::pair<int, int> edge)
        { return edge.first == firstNodeIndex; }), nodes[secondNodeIndex].edges.end());
}

MST Graph::getMST()
{
    MST resultMST;
    std::vector<std::tuple<int, int, int>> edges;// (from, to, weight)
    for (const auto& node : nodes)
    {
        for (auto edge : node.edges)
            if (node.index < edge.first) 
            edges.emplace_back(node.index,edge.first, edge.second);
    }
    //edges现在含有所有的边

    std::sort(edges.begin(), edges.end(), [](std::tuple<int, int, int> a, std::tuple<int, int, int> b)
    {
        return std::get<2>(a) < std::get<2>(b);
    });
    
    //edges含有已排序的所有的边，现在根据这个排序关系，选择权重小且不形成环的边，加入最小生成树
    //对于每一个结点，进行root的维护，以确保不形成环
    std::vector<int> parent(nodes.size());
    for (size_t i = 0; i < nodes.size(); i++) {
        parent[i] = i; // Each node starts as its own parent
    }
    //首先让每个结点的parent指向自身

    std::function<int(int)> find = [&parent, &find](int index)->int
    {
        if (index != parent[index])
            parent[index] = find(parent[index]);
        return parent[index];
    };
    //递归寻找头parent的lambda


    while (resultMST.edges.size() < nodes.size()-1)
    {
        auto front = edges.front();
        if (find(std::get<0>(front)) != find(std::get<1>(front)))
        {
            //加入最小生成树
            resultMST.addEdge(std::get<0>(front), std::get<1>(front), std::get<2>(front));
            //加入最小生成树之后需要立即更新parent
            parent[std::get<0>(front)] = std::get<1>(front);
        }
        //无论是否加入了最小生成树，此时都应当擦除
        edges.erase(edges.begin());
    }
    
    return resultMST;
}

//根据最小生成树（大量边的集合，这些边标定了整个最小生成树）作为启发式函数
//进行A*搜索

