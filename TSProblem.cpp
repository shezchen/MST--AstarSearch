//
// Created by aoyeningluosi on 25-3-18.
//
#include "header.h"
#include <random>

TSProblem::TSProblem(int nodesNumber): graph(std::vector<Node>())
{
    std::vector<Node> nodes;
    for (int i = 0; i < nodesNumber; i++)
    {
        nodes.emplace_back(i);
    }

    graph = Graph(nodes);

    // 随机数生成
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> weightDist(1, 100); // 权重范围1-100

    // 随机选择起始节点
    std::uniform_int_distribution<> nodeDist(0, nodesNumber-1);
    startNodeIndex = nodeDist(gen);

    // 随机生成边
    int edgesPerNode = 3; // 每个节点最多边数
    for (int i = 0; i < nodesNumber; i++) {
        for (int j = 0; j < edgesPerNode; j++) {
            int to = nodeDist(gen); // 随机目标节点

            while (to==i)
                to = nodeDist(gen);

            if (to != i) { // 不创建自环
                int weight = weightDist(gen);
                graph.addEdge(i, to, weight);
            }
        }
    }
}
