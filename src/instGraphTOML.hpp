#ifndef instGraphTOML_hpp
#define instGraphTOML_hpp

#include "instGraph.hpp"

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
class instGraphTOML : public instGraph
{

public:

    /// Default c'tor
    instGraphTOML();

    ~instGraphTOML();
   
    int parseTOMLTable( toml::v3::table & tbl );

    int loadTOMLFile( const std::string & fname );

}; //class instGraphTOML

}; //namespace ingr


#endif //instGraphTOML_hpp
