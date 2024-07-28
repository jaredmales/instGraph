#ifndef instGraphXML_hpp
#define instGraphXML_hpp

#include "instGraph.hpp"

//forward
namespace pugi
{
    class xml_document;
}

namespace ingr
{

/// A class to hold an instrument graph.
class instGraphXML : public instGraph
{

protected:

    pugi::xml_document * m_doc {nullptr};

    int parseXMLDoc( std::string & emsg );

public:

    /// Default c'tor
    instGraphXML();

    ~instGraphXML();
   
    int parseXMLDoc( std::string & emsg,
                     const pugi::xml_document & doc
                   );

    int loadXMLFile( std::string & emsg,
                     const std::string & fname 
                   );


}; //class instGraphXML

}; //namespace ingr


#endif //instGraphXML_hpp
