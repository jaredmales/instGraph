#include "instIOPut.hpp"
#include "instNode.hpp"
#include "instBeam.hpp"
#include "instGraph.hpp"

#include <iostream>

namespace ingr
{

instIOPut::instIOPut()
{
    makeKey();
}

instIOPut::instIOPut( instNode* node,
                      ioDir io,
                      std::string name,
                      putType type,
                      instBeam* beam
                    )
{
    setup(node, io, name, type, beam);
}

instIOPut::instIOPut(const instIOPut& iop)
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

void instIOPut::setup( instNode* node,
                       ioDir io,
                       std::string name,
                       putType type,
                       instBeam* beam
                     )
{
    m_node = node;
    m_io = io;
    m_name = name;
    m_type = type;
    m_beam = beam;

    makeKey();
}

bool instIOPut::nodeValid() const
{
    return (m_node != nullptr);
}

instNode* instIOPut::node() const
{
    if (m_node == nullptr)
    {
        throw std::out_of_range("instIOPut::node(): attempt to access m_node pointer which is null");
    }

    return m_node;
}

void instIOPut::node(instNode* node)
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
void instIOPut::name(const std::string& nm)
{
    m_name = nm;
    makeKey();
}

putType instIOPut::type() const
{
    return m_type;
}

void instIOPut::type(putType t)
{
    m_type = t;
    makeKey();
}

bool instIOPut::beamValid() const
{
    return (m_beam != nullptr);
}

instBeam* instIOPut::beam() const
{
    if (m_beam == nullptr)
    {
        throw std::out_of_range("instIOPut::beam(): attempt to access m_beam pointer which is null");
    }

    return m_beam;
}

void instIOPut::beam(instBeam* b)
{
    m_beam = b;
}

void instIOPut::outputLinked(const bool& ol)
{
    m_outputLinked = ol;
}

bool instIOPut::outputLinked() const
{
    return m_outputLinked;
}

putState instIOPut::state() const
{
    return m_state;
}

void instIOPut::state( putState ns,
                       bool nobeam,
                       bool byOutputLink
                     )
{
    //If this is an input and switching on, check if beam is off
    //because otherwise we won't get to waiting
    if (m_io == ioDir::input && ns == putState::on)
    {
        if (m_beam)
        {
            if (m_beam->state() == beamState::off)
            {
                ns = putState::waiting;
            }
        }
    }

    //If this is an output and it is output-linked from an input and this is not being set
    //by that output link, we do nothing.
    if (m_io == ioDir::output && m_outputLinked == true && byOutputLink == false)
    {
        return;
    }

    bool changed = false;

    if (m_state != ns)
    {
        changed = true;
    }

    m_state = ns;

    if (changed)
    {
        if (m_parentGraph) m_parentGraph->stateChange();
    }

    //If an input and it is linked, propagate to the linked outputs
    if (m_io == ioDir::input && m_outputLinks.size() > 0 && m_node != nullptr)
    {
        for (auto&& oit : m_outputLinks)
        {
            if (!m_node->outputValid(oit))
            {
                throw std::logic_error("instIOPut::state: outputLink is invalid");
            }

            m_node->checkOutputLinks(oit);
        }
    }

    if (m_beam != nullptr && !nobeam)
    {
        m_beam->stateChange();
    }
}

void instIOPut::parentGraph(instGraph* ig)
{
    m_parentGraph = ig;
}

std::string instIOPut::key() const
{
    return m_key;
}

void instIOPut::outputLink(const std::string& ol)
{
    if (m_io != ioDir::input)
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

const std::set<std::string>& instIOPut::outputLinks()
{
    return m_outputLinks;
}

bool instIOPut::auxDataValid()
{
    return (m_auxData != nullptr);
}

void* instIOPut::auxData()
{
    if (m_auxData == nullptr)
    {
        throw std::out_of_range("instIOPut::auxData(): attemmpt to accesss m_auxData pointer which is null");
    }

    return m_auxData;
}

void instIOPut::auxData(void* ad)
{
    m_auxData = ad;
}

void instIOPut::stateChange()
{
    if (m_node == nullptr) return;

    m_node->stateChange();
}

void instIOPut::makeKey()
{
    m_key = m_name;
}

} //namespace instGraph
