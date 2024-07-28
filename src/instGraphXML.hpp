#ifndef instGraphXML_hpp
#define instGraphXML_hpp

#include "instGraph.hpp"

//forward
namespace pugi
{
    class xml_document;
    class xml_node;
}

namespace ingr
{

/// Specifying an instrument graph with the drawio XML format.
/**
  * \ingroup explainer 
  */
class instGraphXML : public instGraph
{

public:

    struct guiData
    {
        pugi::xml_node* xmlNode {nullptr};

        std::string styleValue;

        size_t strokeColorPos {std::string::npos};

        size_t fontColorPos {std::string::npos};

        guiData(pugi::xml_node * xn);

        ~guiData();

        void findColors();

        void strokeColor(const std::string & color);

        void fontColor(const std::string & color);
    };

    typedef guiData auxDataT;

protected:

    pugi::xml_document * m_doc {nullptr};

    int parseXMLDoc( std::string & emsg );

    std::string m_colorOff {"FF0000"};
    std::string m_colorOn  {"00FF00"};
    std::string m_colorInt {"FFFF00"};

    std::string m_outputPath {"tmp.drawio"}; ///< The output file path for writing updated drawio xml.

public:

    /// Default c'tor
    instGraphXML();

    /// Desctructor
    ~instGraphXML();
   
    int parseXMLDoc( std::string & emsg,
                     const pugi::xml_document & doc
                   );

    int loadXMLFile( std::string & emsg,
                     const std::string & fname 
                   );

    /// Get the output file path for writing updated drawio xml
    /**
      * \returns a const reference to m_outputPath 
      */
    const std::string & outputPath();

    /// Set the output file path for writing updated drawio xml
    void outputPath( const std::string & op /**< [in] the new output path */);

    virtual void stateChange();
    
}; //class instGraphXML

}; //namespace ingr


#endif //instGraphXML_hpp
