#include "instNode.hpp"
#include "instIOPut.hpp"
#include "instBeam.hpp"

#include <iostream>
#include <stdexcept>

namespace ingr 
{

instNode::instNode( const instNode & in )
{
    m_name = in.m_name;
    m_inputs = in.m_inputs;
    m_outputs = in.m_outputs;
}

instNode::~instNode()
{
    for(auto && iput : m_inputs)
    {
        delete iput.second;
    }

    for(auto && oput : m_outputs)
    {
        delete oput.second;
    }
}

std::string instNode::name() const
{
   return m_name;
}

void instNode::name(const std::string & nn /**< [in] */)
{
   m_name = nn;
}

std::string instNode::key()
{
   return m_name;
}

std::string instNode::addIOPut( instIOPut * ip )
{
    if(ip == nullptr)
    {
        throw std::invalid_argument("instNode::addIOPut nullptr");
    }

    if(ip->io() == ioDir::input ) 
    {
        std::cerr << "\tadding output: \"" << ip->name() << "\"\n";
        if(ip->beam() != nullptr) std::cerr << "\t         beam: \"" << ip->beam()->name() << "\"\n";
        else std::cerr << "\t         beam: <null> \n";

        std::pair<ioputMapT::iterator, bool> res = m_inputs.insert({ip->key(), ip});
    
        if(res.second == false) 
        {
            ///\todo test me
  
            std::cerr << "input already exists\n";
            return res.first->first; //return the key
        }
  
        instIOPut * newPut = res.first->second;
  
        if(newPut->beam() != nullptr) 
        {
            newPut->beam()->dest(ip);
        }

        return res.first->first; //return the key

    }
    else if(ip->io() == ioDir::output ) 
    {
        std::cerr << "\tadding output: \"" << ip->name() << "\"\n";
        if(ip->beam() != nullptr) std::cerr << "\t         beam: \"" << ip->beam()->name() << "\"\n";
        else std::cerr << "\t         beam: <null> \n";
  
        std::pair<ioputMapT::iterator, bool> res = m_outputs.insert({ip->key(), ip});
  
        if(res.second == false) 
        {
            ///\todo test me
   
            std::cerr << "input already exists\n";
            return res.first->first; //return the key
        }
  
        instIOPut * newPut = res.first->second;
  
        if(newPut->beam() != nullptr) 
        {
            newPut->beam()->source(newPut);
        }

        return res.first->first; //return the key

    }
    else 
    {
        //shouldn't be able to get here
        throw std::invalid_argument("instNode::addIOPut invalid io type");
    }

    return "";
}

const instNode::ioputMapT & instNode::inputs() const
{
   return m_inputs;
}

instIOPut * instNode::input( const std::string & key)
{
    if(m_inputs.count(key) != 1)
    {
        std::string msg = "unknown input with key \"";
        msg += key + "\"";
        msg += " (ingr::instNode::input ";
        msg += __FILE__;
        msg += " ";
        msg += __LINE__;
        msg += ")";

        throw std::invalid_argument(msg);
    };

    return m_inputs[key];
}

const instNode::ioputMapT & instNode::outputs() const
{
   return m_outputs;
}

instIOPut * instNode::output( const std::string & key)
{
    if(m_outputs.count(key) != 1)
    {
        std::string msg = "unknown output with key \"";
        msg += key + "\"";
        msg += " (ingr::instNode::output ";
        msg += __FILE__;
        msg += " ";
        msg += std::to_string(__LINE__);
        msg += ")";

        throw std::invalid_argument(msg);
    };

    return m_outputs[key];
}

void instNode::stateChange()
{

}

}
