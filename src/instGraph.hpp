
#include <map>
#include <string>

#include "instNode.hpp"
#include "instBeam.hpp"

//forward decl
namespace toml
{
namespace v3
{
    class table;
}
}

namespace ingr
{

/// A class to hold an instrument graph.
class instGraph
{

public:
    typedef std::map<std::string, ingr::instNode> nodeMapT;

    typedef std::map<std::string, ingr::instBeam> beamMapT;

protected:

    /// The nodes of this graph
    nodeMapT m_nodes;

    /// The beams of this graph
    beamMapT m_beams;

public:

    /// Default c'tor
    instGraph();

    const nodeMapT & nodes();

    instNode & node( const std::string & key );

    const beamMapT & beams();

    instBeam & beam( const std::string & key );
   
    int constructFromTOMLTable( toml::v3::table & tbl );

    int constructFromTOMLFile( const std::string & fname );


}; //class instGraph

}; //namespace ingr
