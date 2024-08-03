
#include <iostream>

#include "toml++/toml.h"

#include "instGraph.hpp"

#if 0
   #define BREADCRUMB std::cerr << __FILE__ << " " << __LINE__ << std::endl;
#else
   #define BREADCRUMB 
#endif

namespace ingr
{

instGraph::instGraph()
{
}

instGraph::~instGraph()
{
    for(auto && beam : m_beams)
    {
        delete beam.second;
    }

    for(auto && node : m_nodes)
    {
        delete node.second;
    }
}

const instGraph::nodeMapT & instGraph::nodes()
{
    return m_nodes;
}

bool instGraph::nodeValid( const std::string & key) const 
{
    if(m_nodes.count(key) != 1)
    {
        return false;
    }
    
    return(m_nodes.at(key) != nullptr);

}

instNode * instGraph::node( const std::string & key )
{
    if(m_nodes.count(key) != 1)
    {
        std::string msg = "unknown node with key \"";
        msg += key + "\"";
        msg += " (ingr::instGraph::node ";
        msg += __FILE__;
        msg += " ";
        msg += std::to_string(__LINE__);
        msg += ")";

        throw std::invalid_argument(msg);
    };

    if(m_nodes[key] == nullptr)
    {
        throw std::out_of_range("instGraph::node() attempt to access m_nodes item pointer which is null");
    }

    return m_nodes[key];
}

const instGraph::beamMapT & instGraph::beams()
{
    return m_beams;
}

bool instGraph::beamValid( const std::string & key) const 
{
    if(m_beams.count(key) != 1)
    {
        return false;
    }
    
    return(m_beams.at(key) != nullptr);

}

instBeam * instGraph::beam( const std::string & key )
{
    if(m_beams.count(key) != 1)
    {
        std::string msg = "unknown node with key \"";
        msg += key + "\"";
        msg += " (ingr::instGraph::beam ";
        msg += __FILE__;
        msg += " ";
        msg += std::to_string(__LINE__);
        msg += ")";

        throw std::invalid_argument(msg);
    };

    if(m_beams[key] == nullptr)
    {
        throw std::out_of_range("instNode::beam() attempt to access m_beams item pointer which is null");
    }

    return m_beams[key];
}

void instGraph::stateChange()
{
}

} //namespace ingr

