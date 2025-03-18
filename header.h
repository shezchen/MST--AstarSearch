#pragma once
#include <memory>
#include <vector>

struct Node
{
    int index;
    std::vector<std::pair<int, int>> edges;
    Node(int index) : index(index) {}
};
struct MST;
struct Graph
{
    std::vector<Node> nodes;
    Graph(std::vector<Node> nodes):nodes(nodes){}

    void addEdge(int firstNodeIndex,int secondNodeIndex,int weight);
    void removeEdge(int firstNodeIndex,int secondNodeIndex);

    MST getMST();//得到最小生成树
};
struct AstarNode:std::enable_shared_from_this<AstarNode>
{
    //A*搜索的结点应该维护此结点处已访问的城市列表，以及到达此结点的路径长度
    //以及当前旅行商在的城市结点
    //以及使用最小生成树MST得到的启发式函数的紧下界

    std::vector<int> graphNodesVisitedIndex;//已访问的城市列表的index(构造)
    int pathLength;//到达此结点的路径长度（构造）
    int currentCityIndex;//当前旅行商在的城市结点的index（构造）
    int lowerBound;//使用最小生成树MST得到的启发式函数的紧下界（构造）

    int allCost;//到达此结点的路径长度+启发式函数的紧下界（构造）

    std::vector<int> history;//在此结点所示的情况下，城市index的访问历史
    AstarNode(std::vector<int> graphNodesVisitedIndex, int pathLength, int currentCityIndex, int lowerBound);
};



class TSProblem
{
public:
    int startNodeIndex;
    Graph graph;
    TSProblem(int nodesNumber);//随机生成含有nodesNumber个节点的图，随机指定一个节点作为起始节点
};


struct AstarSearch
{
    bool searchIsSolved = false;
    std::shared_ptr<AstarNode> finalNode=nullptr;
    
    std::vector<std::shared_ptr<AstarNode>> AstarNodes;//此处时A*搜索的全部结点
    TSProblem problem;
    
    AstarSearch(TSProblem problem);

    std::vector<std::shared_ptr<AstarNode>> openList;//开放列表存储指向所有正在被延展的AstarNode的指针
    void sortOpenList();//对开放列表进行排序
    void expandNode(std::shared_ptr<AstarNode> node);//扩展结点
    //如果扩展结点时发现所有的城市都已经访问过了searchIsSolved=true，finalNode=找到答案的状态结点
};


struct MST {
    std::vector<std::tuple<int, int, int>> edges; // (from, to, weight)
    int totalWeight;
    
    MST() : totalWeight(0) {}
    
    void addEdge(int from, int to, int weight) {
        edges.emplace_back(from, to, weight);
        totalWeight += weight;
    }
};