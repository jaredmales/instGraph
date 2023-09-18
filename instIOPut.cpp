#include "instIOPut.hpp"
#include "instNode.hpp"
#include "instBeam.hpp"

#include <iostream>

namespace ingr
{

instIOPut::instIOPut()
{
    makeKey();
}

instIOPut::instIOPut( instNode * node,
                      ioDir io,
                      std::string name,
                      putType type,
                      instBeam * beam
                    )
{
    setup(node, io, name, type, beam);
}

instIOPut::instIOPut( const instIOPut & iop )
{
    m_node = iop.m_node;
    m_io = iop.m_io;
    m_name = iop.m_name;
    m_type = iop.m_type;
    m_beam = iop.m_beam;
    m_state = iop.m_state;
    m_key = iop.m_key;
    m_outputLinks = iop.m_outputLinks;
}

void instIOPut::setup( instNode * node,
                       ioDir io,
                       std::string name,
                       putType type,
                       instBeam * beam
                     )
{
    m_node = node;
    m_io = io;
    m_name = name;
    m_type = type;
    m_beam = beam;

    makeKey();
}

instNode * instIOPut::node() const
{
    return m_node;
}

void instIOPut::node( instNode * node)
{
   m_node = node;
   makeKey();
}

ioDir instIOPut::io() const
{
   return m_io;
}
void instIOPut::io(ioDir iot)
{
   m_io = iot;
   makeKey();
}

std::string instIOPut::name() const
{
   return m_name;
}
void instIOPut::name( const std::string & nm )
{
   m_name = nm;
   makeKey();
}

putType instIOPut::type() const
{
   return m_type;
}

void instIOPut::type( putType t )
{
   m_type = t;
   makeKey();
}

instBeam * instIOPut::beam() const
{
   return m_beam;
}

void instIOPut::beam( instBeam * b)
{
   m_beam = b;
}

putState instIOPut::state() const
{
   return m_state;
}

void instIOPut::state( putState ns,
                       bool nobeam
                     )
{
    //If this is an input and switching on, check if beam is off
    //because otherwise we won't get to waiting
    if(m_io == ioDir::input && ns == putState::on)
    {
        if(m_beam)
        {
            if(m_beam->state() == beamState::off)
            {
                ns = putState::waiting;
            }
        }
    }

    m_state = ns;

    //If an input and it is linked, propagate to the linked outpus
    if(m_io == ioDir::input && m_outputLinks.size() > 0 && m_node)
    {
        for(auto && oit : m_outputLinks)
        {
            m_node->output(oit).state(ns);
        }
    }

    if(m_beam != nullptr && !nobeam)
    {
        m_beam->stateChange();
    }
}

std::string instIOPut::key() const
{
   return m_key;
}

void instIOPut::outputLink(const std::string & ol)
{
    if(m_io != ioDir::input)
    {
        std::string msg = "attempt to add outputLink to output ";
        msg += m_name + " " + ol + " (ingr::instIOPUt::outputlink ";
        msg += __FILE__;
        msg += " ";
        msg += __LINE__;
        msg += ")";

        throw std::logic_error(msg);
    }
    m_outputLinks.insert(ol);
}

const std::set<std::string> & instIOPut::outputLinks()
{
   return m_outputLinks;
}

void instIOPut::stateChange()
{
   if(m_node == nullptr) return;

   m_node->stateChange();
}

void instIOPut::makeKey()
{
    m_key = m_name;

    /*if(m_node != nullptr) m_key = m_node->key();
    m_key += '.';

    m_key += ioDir2Char(m_io);
    m_key += '.';

    m_key += putType2Char(m_type);
    m_key += '.';

    m_key += m_name;*/
}

} //namespace instGraph
