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
        std::pair<ioputMapT::iterator, bool> res = m_inputs.insert({ip->key(), ip});
    
        if(res.second == false) 
        {
            ///\todo test me
  
            std::cerr << "input already exists\n";
            return res.first->first; //return the key
        }
  
        instIOPut * newPut = res.first->second;
  
        if(newPut->beamValid()) 
        {
            newPut->beam()->dest(ip);
        }

        return res.first->first; //return the key

    }
    else if(ip->io() == ioDir::output ) 
    {  
        std::pair<ioputMapT::iterator, bool> res = m_outputs.insert({ip->key(), ip});
  
        if(res.second == false) 
        {
            std::cerr << "input already exists\n";
            return res.first->first; //return the key
        }
  
        instIOPut * newPut = res.first->second;
  
        if(newPut->beamValid()) 
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

bool instNode::inputValid( const std::string & key) const 
{
    if(m_inputs.count(key) != 1)
    {
        return false;
    }
    
    return(m_inputs.at(key) != nullptr);

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

    if(m_inputs[key] == nullptr)
    {
        throw std::out_of_range("instNode::input() attempt to access m_inputs item pointer which is null");
    }

    return m_inputs[key];
}

const instNode::ioputMapT & instNode::outputs() const
{
   return m_outputs;
}

bool instNode::outputValid( const std::string & key) const 
{
    if(m_outputs.count(key) != 1)
    {
        return false;
    }
    
    return(m_outputs.at(key) != nullptr);

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

    if(m_outputs[key] == nullptr)
    {
        throw std::out_of_range("instNode::output() attempt to access m_outputs item pointer which is null");
    }

    return m_outputs[key];
}

void instNode::updateOutputLinks()
{
    //First turn them all off in case one has been unlinke
    for(auto && op : m_outputs)
    {
        if(op.second == nullptr) 
        {
            continue;
        }

        op.second->outputLinked(false);
    }

    //Now check outputLinks of all inputs
    for(auto && ip : m_inputs)
    {
        if(ip.second == nullptr)
        {
            continue;
        }

        for(auto && ol : ip.second->outputLinks()) //loop over the output links of this input
        {
            try 
            {
                instIOPut * op = output(ol);
                op->outputLinked(true);
            }
            catch(const std::invalid_argument & e)
            {
                std::string msg = "exception caught at ";
                msg += std::string(__FILE__) + " " + std::to_string(__LINE__) + ":\n   " + e.what();
                throw std::invalid_argument(msg);
            }
            catch(const std::out_of_range & e)
            {
                std::string msg = "exception caught at ";
                msg += std::string(__FILE__) + " " + std::to_string(__LINE__) + ":\n   " + e.what();
                throw std::out_of_range(msg);
            }
            catch(const std::exception& e)
            {
                std::string msg = "other exception caught at ";
                msg += std::string(__FILE__) + " " + std::to_string(__LINE__) + ":\n   " + e.what();
                throw std::logic_error(msg);
            }
            catch(...)
            {
                std::string msg = "unknown exception caught at ";
                msg += std::string(__FILE__) + " " + std::to_string(__LINE__);
                throw std::logic_error(msg);
            }

        }
    }
}


void instNode::checkOutputLinks( const std::string op )
{
    if(!outputValid(op)) //don't bother if this output is bad
    {
        return;
    }

    putState ps = putState::off;

    for(auto && ip : m_inputs) //Check the output links of each input
    {
        if(ip.second == nullptr)
        {
            continue;
        }

        for(auto && ol : ip.second->outputLinks()) //loop over the output links of this input
        {
            if(ol == op) //This is the output link we're looking for, so this input is linked to it
            {
                //Set to on if it's on, waiting if it's off.
                if(ip.second->state() == putState::on)
                {
                    ps = putState::on;
                }
                else if(ps == putState::off && ip.second->state() == putState::waiting)
                {
                    ps = putState::waiting;
                }
            }
        }
    }

    try 
    {
        instIOPut * optr = output(op);
        optr->state(ps, false, true); 
    }
    catch(const std::invalid_argument & e)
    {
        std::string msg = "exception caught at ";
        msg += std::string(__FILE__) + " " + std::to_string(__LINE__) + ":\n   " + e.what();
        throw std::invalid_argument(msg);
    }
    catch(const std::out_of_range & e)
    {
        std::string msg = "exception caught at ";
        msg += std::string(__FILE__) + " " + std::to_string(__LINE__) + ":\n   " + e.what();
        throw std::out_of_range(msg);
    }
    catch(const std::exception& e)
    {
        std::string msg = "other exception caught at ";
        msg += std::string(__FILE__) + " " + std::to_string(__LINE__) + ":\n   " + e.what();
        throw std::logic_error(msg);
    }
    catch(...)
    {
        std::string msg = "unknown exception caught at ";
        msg += std::string(__FILE__) + " " + std::to_string(__LINE__);
        throw std::logic_error(msg);
    }
}

bool instNode::auxDataValid()
{
    return (m_auxData != nullptr);
}

void * instNode::auxData()
{
    if(m_auxData == nullptr)
    {
        throw std::out_of_range("instNode::auxData(): attemmpt to accesss m_auxData pointer which is null");
    }

    return m_auxData;
}

void instNode::auxData( void * ad)
{
    m_auxData = ad;
}
    

void instNode::stateChange()
{

}

}
