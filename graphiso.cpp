#include <iostream>
#include <sstream>
#include "graphiso.h"

using namespace std;

// ================= Complete - Begin Graph class implementation ===================
Graph::Graph(std::istream& istr)
{
    string aline;
    while(getline(istr,aline))
    {
        istringstream iss(aline);
        string u, v;
        if(iss >> u){
            VERTEX_SET_T neighbors;
            while(iss >> v)
            {
                neighbors.insert(v);
            }
            adj_.insert(std::make_pair(u,neighbors));
        }
    }
}


bool Graph::edgeExists(const VERTEX_T& u, const VERTEX_T& v) const
{
    if((adj_.find(u) != adj_.end()) && (adj_.at(u).find(v) != adj_.at(u).end()))
    {
        return true;
    }
    return false;
}
const VERTEX_SET_T& Graph::neighbors(const VERTEX_T& v) const
{
    if(adj_.find(v) == adj_.end()){
        throw std::invalid_argument("Neighbors: invalid vertex - not in  map");
    }
    return adj_.at(v);
}
VERTEX_LIST_T Graph::vertices() const
{
    VERTEX_LIST_T verts;
    for(const auto& p : adj_)
    {
        verts.push_back(p.first);
    }
    return verts;
}
// ================= Complete - End Graph class implementation ===================

// Prototype and helper functions here

// To be completed
bool isConsistent(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    // Feel free to change or remove this starter code
    VERTEX_LIST_T g1verts = g1.vertices();

    //go through every vertices in g1
    for(size_t i=0; i<g1verts.size(); i++)
    {
        //check if there is mapping for current vertice in g1
        VERTEX_T vG1 = g1verts[i];
        std::pair<VERTEX_T, VERTEX_T> * vG1pair = mapping.find(vG1);
        //there is no mapping
        if(vG1pair == nullptr)
        {
            continue;
        }
        else
        {
            //the mapped vertex for vG1 in g2
            VERTEX_T mvG1 = vG1pair->second;
            
            //the set of neighbors for vG1
            //the idea is that since vG1 has edge with its neighbor, mvG1 should have edge with the mapped neighbor of vG1
            const VERTEX_SET_T sNeighbor1 = g1.neighbors(vG1);
            const VERTEX_SET_T sNeighbor2 = g2.neighbors(mvG1);
            if(sNeighbor1.size() != sNeighbor2.size())
            {
                return false;
            }
            
            //iterate through every neighbor of vG1
            for(std::set<std::string>::iterator It = sNeighbor1.begin(); It!=sNeighbor1.end(); ++It)
            {
                
                std::pair<VERTEX_T, VERTEX_T> * nvG1pair = mapping.find(*It);
                if(nvG1pair == nullptr)
                {
                    continue;
                }
                VERTEX_T mnvG1 = nvG1pair->second;
                
                if(g1.edgeExists(vG1, *It)==true && g2.edgeExists(mvG1, mnvG1)==true )
                {
                    continue;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    
    return true;
    
}

// Add any helper functions you deem useful



bool graphIsoHelper(const Graph& g1, VERTEX_LIST_T g1verts, const Graph& g2, VERTEX_ID_MAP_T& mapping, long unsigned int idx1,  VERTEX_SET_T usedG2)
{
    
    bool status = false;
    
    VERTEX_T vG1 = g1verts[idx1];
    VERTEX_LIST_T g2verts = g2.vertices();
    
    //map vG1 with each node in g2
	for(size_t i=0; i<g2verts.size(); i++)
    {
        VERTEX_T vG2 = g2verts[i];
        
        //if the node in g2 is not mapped
        if(usedG2.find(vG2) == usedG2.end())
        {
            std::pair<VERTEX_T, VERTEX_T> curr = std::make_pair(vG1, vG2);
            mapping.insert(curr);
            
            if(isConsistent(g1,g2,mapping) == false)
            {
                mapping.remove(curr.first);
            }
            else
            {
                if(idx1 == g1verts.size()-1)
                {
                    return true;
                }
                VERTEX_SET_T tmpSet = usedG2;
                tmpSet.insert(vG2);
                status = graphIsoHelper(g1, g1verts, g2, mapping, idx1+1, tmpSet);
            }
            
            if(status == true)
            {
                break;
            }
        }
        
    }
    return status;
}

// To be completed
bool graphIso(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping)
{
    VERTEX_LIST_T g1verts = g1.vertices();
    if(g1verts.size() != g2.vertices().size())
    {
        return false;
    }
    // Add code here
    VERTEX_SET_T usedG2;
    return graphIsoHelper(g1, g1verts, g2, mapping, 0, usedG2);
}
