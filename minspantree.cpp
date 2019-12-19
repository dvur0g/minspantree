#include <iostream>
#include <fstream>
#include <string>
#include <limits>

class ConnectionList
{
    public:
        struct element
        {
            std::string node_one, node_two;
            int weight;

            element *next, *prev;

            element(std::string node1, std::string node2, int w)
            {
                node_one = node1;
                node_two = node2;
                weight = w;
            }
        } *root = NULL;
        
        void add(std::string node_one, std::string node_two, int weight)
        {
            if(node_one == node_two)
                return;

            if(!root)
            {
                root = new element(node_one, node_two, weight);
                root -> next = NULL;
                return;
            }

            element *t = new element(node_one, node_two, weight);
            element *p = root;
            while(true)
            {
                if(p -> node_one == node_one && p -> node_two == node_two || p -> node_two == node_one && p -> node_one == node_two)
                    return;
                
                if(weight <= p -> weight)
                {  
                    if(p == root)
                    {
                        t -> next = p;
                        p -> prev = t;
                        root = t;
                        t -> prev = NULL;
                        return;
                    }
                    t -> prev = p -> prev;
                    t -> next = p;
                    
                    p -> prev -> next = t;
                    p -> prev = t;
                    return;
                }
                else if(p -> next)
                    p = p -> next;
                else
                {
                    p -> next = t;
                    t -> prev = p;
                    t -> next = NULL;
                    return;  
                }
            }      
        }

        void print()
        {
            element *p = root;
            while(p)
            {
                std::cout << p -> node_one << " " << p -> node_two << " " << p -> weight << "\n";
                p = p -> next;
            }
        }

        element* pop()
        {
            element *p = root;
            root = root -> next;
            return p;
        }

        bool isEmpty()
        {
            return root;
        }
};

class Teams
{
    public:
        struct element
        {
            std::string node;
            int team;

            element(std::string &node_name)
            {
                node = node_name;
                team = 0;
            }

            element *next;
        } *root = NULL;
        
        int last_team = 0;
        
        void addNode(std::string node_name)
        {
            if(!root)
            {
                root = new element(node_name);
                root -> next = NULL;
                return;
            }

            element *p = root;
            while(p)
            {
                if(p -> node == node_name)
                    return;

                if(p -> next)
                    p = p -> next;
                else 
                    break;
            }
            p -> next = new element(node_name);
            p -> next -> next = NULL;
        }   

        element* getNode(std::string node_name)
        {
            element *p = root;
            while(p)
            {
                if(p -> node == node_name)
                    return p;
                p =  p -> next;
            }
            return {0};
        }

        void changeTeam(int old_team, int new_team)
        {
            element *p = root;
            while(p)
            {
                if(p -> team == old_team)
                    p -> team = new_team;
                p = p -> next;
            }
        }

        void print()
        {
            element *p = root;
            while(p)
            {   
                std::cout << p -> node << " " << p -> team << ", ";
                p = p -> next;
            } std::cout << "\n";
        }
};

void kruskal(ConnectionList &list, Teams &teams)
{
    ConnectionList::element *el;
    int weight = 0;
    while(list.isEmpty())
    {
        el = list.pop();
        Teams::element *first = teams.getNode(el -> node_one);
        Teams::element *second = teams.getNode(el -> node_two);

        if(first -> team != second -> team || first -> team == 0 && second -> team == 0)
        {
            std::cout << el -> node_one << " " << el -> node_two << "\n"; 
            weight += el -> weight;
        }
        else
            continue;

        if(first -> team == 0 && second -> team == 0)
        {
            first -> team = ++teams.last_team;
            second -> team = teams.last_team;
        }
        else if(first -> team != second -> team)
            if(first -> team != 0)
                if(second -> team == 0)
                    second -> team = first -> team;
                else
                    teams.changeTeam(second -> team, first -> team);
            else
               if(first -> team == 0)
                    first -> team = second -> team;
                else
                    teams.changeTeam(first -> team, second -> team);
        delete el;
    }
    std::cout << weight;
}

int main()
{
    ConnectionList clist;
    Teams teams;

    std::ifstream file;
    file.open("input.txt");
    
    std::string input_string, temp, temp_name1, temp_name2;
    while (file >> temp)
    {
        temp_name1 = temp;
        if(file >> temp)
            temp_name2 = temp;
        if(file >> temp)
        {
            clist.add(temp_name1, temp_name2, std::stoi(temp));
            teams.addNode(temp_name1);
            teams.addNode(temp_name2);
        }
    }   
    file.close();

    kruskal(clist, teams);

    clist.print();
    return 0;
}