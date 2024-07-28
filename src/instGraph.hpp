#ifndef instGraph_hpp
#define instGraph_hpp

#include <map>
#include <string>

#include "instNode.hpp"
#include "instBeam.hpp"


namespace ingr
{

/// A class to hold an instrument graph.
class instGraph
{

public:
    typedef std::map<std::string, ingr::instNode *> nodeMapT;

    typedef std::map<std::string, ingr::instBeam *> beamMapT;

protected:

    /// The nodes of this graph
    nodeMapT m_nodes;

    /// The beams of this graph
    beamMapT m_beams;

public:

    /// Default c'tor
    instGraph();

    ~instGraph();

    const nodeMapT & nodes();

    const bool nodeValid( const std::string & key ) const;

    instNode * node( const std::string & key );

    const beamMapT & beams();

    const bool beamValid( const std::string & key ) const;

    instBeam * beam( const std::string & key );
   
    


}; //class instGraph

}; //namespace ingr


#endif //instGraph_hpp
