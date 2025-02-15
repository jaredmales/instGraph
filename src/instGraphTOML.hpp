#ifndef instGraphTOML_hpp
#define instGraphTOML_hpp

#include "instGraph.hpp"

// forward decl
namespace toml
{
namespace v3
{
class table;
}
} // namespace toml

namespace ingr
{

/// Specifying an instrument graph with TOML.
/**
 * \ingroup explainer
 */
class instGraphTOML : public instGraph
{

  public:
    /// Default c'tor
    instGraphTOML();

    ~instGraphTOML();

    int parseTOMLTable( toml::v3::table &tbl );

    int loadTOMLFile( const std::string &fname );

}; // class instGraphTOML

}; // namespace ingr

#endif // instGraphTOML_hpp
