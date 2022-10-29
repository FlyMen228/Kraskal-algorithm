#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <set>
#include <array>
#include <stack>

/*==============================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyMatrixFromFile();

bool checkForConnectivity(const std::vector <std::vector <int> >& adjacencyMatrix);

std::set <std::array <int, 3> > createEdges(const std::vector <std::vector <int> >& adjacencyMatrix);

void kraskalAlgorithm(const int& adjacencyMatrixSize, std::set <std::array<int, 3> >& edges);

bool findCycle(std::vector <std::set <int> >& checkCycles, const int& vertex);

void writeToOutputFile(const std::vector <std::pair <int, int> >& output, const int& weight);

/*==============================================================================================================*/

int main()
{
    std::vector <std::vector <int> > adjacencyMatrix = inputAdjacencyMatrixFromFile();

    if (!checkForConnectivity(adjacencyMatrix))
    {
        std::cout << "The graph is disconnected!";
        return 0;
    }

    std::set <std::array <int, 3> > edges = createEdges(adjacencyMatrix);

    kraskalAlgorithm(adjacencyMatrix.size(), edges);
}

/*==============================================================================================================*/

std::vector <std::vector <int> > inputAdjacencyMatrixFromFile()
{
    std::ifstream reader("test6.txt"); //test1.txt - test6.txt

    if (reader.is_open())
    {
        int numOfVertexes;

        reader >> numOfVertexes;

        std::vector < std::vector <int > > adjacencyMatrix(numOfVertexes, std::vector <int>(numOfVertexes));

        for (int i = 0; i < numOfVertexes; ++i)
            for (int j = 0; j < numOfVertexes; ++j)
                if (!reader.eof())
                    reader >> adjacencyMatrix[i][j];
                else
                    break;

        reader.close();

        return adjacencyMatrix;
    }
    else
    {
        reader.close();

        std::cout << "File Alert!";

        std::exit(0);
    }
}

bool checkForConnectivity(const std::vector <std::vector<int> >& adjacencyMatrix)
{
    std::queue <int> snake;
    std::vector <bool> visitedVertexes(adjacencyMatrix.size(), false);

    visitedVertexes[0] = true;
    snake.push(0);

    while (!snake.empty())
    {
        int vertex = snake.front();
        snake.pop();

        for (int i = 0; i < adjacencyMatrix[vertex].size(); ++i)
            if (adjacencyMatrix[vertex][i] && !visitedVertexes[i])
            {
                snake.push(i);
                visitedVertexes[i] = true;
            }
    }

    return all_of(visitedVertexes.begin(), visitedVertexes.end(), [](bool value) { return value; });
}

std::set <std::array<int, 3> > createEdges(const std::vector <std::vector <int> >& adjacencyMatrix)
{
    std::vector <std::vector <bool> > usedEdges(adjacencyMatrix.size(), std::vector <bool>(adjacencyMatrix[0].size(), false));

    std::set <std::array <int, 3> > edges;

    for (int i = 0; i < adjacencyMatrix.size(); ++i)
        for (int j = 0; j < adjacencyMatrix[i].size(); ++j)
            if (adjacencyMatrix[i][j] && !usedEdges[i][j])
            {
                edges.insert({ adjacencyMatrix[i][j], i, j });
                usedEdges[i][j] = usedEdges[j][i] = true;
            }

    return edges;
}

void kraskalAlgorithm(const int& adjacencyMatrixSize, std::set <std::array <int, 3> >& edges)
{
    std::vector <std::pair <int, int> > output;
    std::vector <std::set <int> > checkCycles(adjacencyMatrixSize);
    std::vector <bool> usedVertexes(adjacencyMatrixSize, false);

    int weight = 0;

    while (any_of(usedVertexes.begin(), usedVertexes.end(), [](bool value) { return !value; }))
        for (const auto& edge : edges)
        {
            std::pair <int, int> rib = { edge[1], edge[2] };

            checkCycles[rib.first].insert(rib.second);
            checkCycles[rib.second].insert(rib.first);

            if (findCycle(checkCycles, rib.first) || findCycle(checkCycles, rib.second))
            {
                checkCycles[rib.first].erase(rib.second);
                checkCycles[rib.second].erase(rib.first);

                continue;
            }

            weight += edge[0];

            output.push_back(rib);

            usedVertexes[rib.first] = usedVertexes[rib.second] = true;

            edges.erase(edge);

            break;
        }

    writeToOutputFile(output, weight);
}

bool findCycle(std::vector <std::set <int> >& checkCycles, const int& vertex)
{
    std::stack <int> cycle;
    cycle.push(vertex);

    std::vector <bool> visitedVertexes(checkCycles.size(), false);

    while (!cycle.empty())
    {
        int currentVertex = cycle.top();

        if (visitedVertexes[currentVertex])
            return true;

        visitedVertexes[currentVertex] = true;

        if (!checkCycles[currentVertex].empty())
        {
            int nextVertex = *begin(checkCycles[currentVertex]);

            cycle.push(nextVertex);

            checkCycles[nextVertex].erase(currentVertex);
            checkCycles[currentVertex].erase(nextVertex);
        }
        else
        {
            cycle.pop();

            if (!cycle.empty())
                visitedVertexes[cycle.top()] = false;
        }
    }

    return false;
}

void writeToOutputFile(const std::vector <std::pair <int, int> >& output, const int& weight)
{
    std::cout << "The weight of the minimum remaining tree is " << weight << "!\nThe minimum remaining tree is written to a file output.txt!\n";

    std::ofstream writer;
    writer.open("output.txt");

    if (writer.is_open())
    {
        writer << "The minimum remaining tree is:\n";

        for (int i = 0; i < output.size(); ++i)
        {
            writer << output[i].first << " - " << output[i].second << std::endl;
        }

        writer.close();
    }
    else
    {
        writer.close();

        std::cout << "File Alert!\n";
    }
}