#ifndef instGraphXML_hpp
#define instGraphXML_hpp

#include <memory>
#include "instGraph.hpp"

// forward
namespace pugi
{
class xml_document;
class xml_node;
} // namespace pugi

namespace ingr
{

/// Specifying an instrument graph with the drawio XML format.
/**
 * \ingroup explainer
 */
class instGraphXML : public instGraph
{

  public:
    struct extraGuiData;

    struct attrCoord
    {
        std::string key;
        size_t keyPos{ std::string::npos };
        size_t valLen{ std::string::npos }; ///< the length after the '=', not including the ';'
        size_t attrLen{ std::string::npos };

        void findKey( const std::string &attr, const std::string &k )
        {
            key = k;
            attrLen = attr.size();
            keyPos = attr.find( key );

            if( keyPos == std::string::npos )
            {
                clear();
                return;
            }

            // make sure it's long enough
            if( attr.size() < keyPos + key.size() + 1 )
            {
                clear();
                return;
            }

            if( attr[keyPos + key.size()] != '=' )
            {
                clear();
                return;
            }

            size_t cpos = attr.find( ';', keyPos + key.size() + 1 );

            if( cpos == std::string::npos )
            {
                clear();
            }

            valLen = cpos - ( keyPos + key.size() + 1 );
        }

        void clear()
        {
            key = "";
            keyPos = std::string::npos;
            valLen = std::string::npos;
            attrLen = std::string::npos;
        }

        ///
        /**
         * \return false if no changes to attr size are made
         * \return true if attr size changes
         */
        bool setValue( std::string &attr, const std::string &value )
        {
            if( attrLen != attr.size() )
            {
                std::string msg = "attrCoord::setValue: attribute length has changed for " + key;
                throw std::runtime_error( msg );
            }

            if( attr.substr( keyPos, key.size() ) != key )
            {
                std::string msg = "attrCoord::setValue: attribute key has changed for " + key;
                throw std::runtime_error( msg );
            }

            if( attr[keyPos + key.size()] != '=' )
            {
                std::string msg = "attrCoord::setValue: parse error: no = for " + key;
                throw std::runtime_error( msg );
            }

            if( attr[keyPos + key.size() + 1 + valLen] != ';' )
            {
                std::string msg = "attrCoord::setValue: attribute value has changed for " + key;
            }

            if( value.size() == valLen )
            {
                for( size_t n = 0; n < valLen; ++n )
                {
                    attr[keyPos + key.size() + 1 + n] = value[n];
                }

                return false;
            }
            else
            {
                attr.erase( keyPos + key.size() + 1, valLen );
                attr.insert( keyPos + key.size() + 1, value );
                valLen = value.size();
                return true;
            }
        }
    };

    struct guiData
    {
        static constexpr char m_defaultColor[] = "#FFFFFF";
        static constexpr int m_defaultOpacity= 100;

        pugi::xml_node *xmlNode{ nullptr };

        std::string styleValue;

        attrCoord strokeColorPos;

        attrCoord fontColorPos;

        attrCoord opacityPos;

        attrCoord textOpacityPos;

        guiData( pugi::xml_node *xn );

        ~guiData();

        void findColors();

        void strokeColor( const std::string &color );

        void fontColor( const std::string &color );

        void opacity( int op );

        void textOpacity( int op );

        void value( const std::string &val );

        std::multimap<std::string, extraGuiData> extraData;
    };

    typedef guiData auxDataT;

    struct extraGuiData
    {
        std::string payload;

        std::shared_ptr<guiData> gdata;
    };

  protected:
    pugi::xml_document *m_doc{ nullptr };

    int parseXMLDoc( std::string &emsg );

    std::string m_defaultColor{ "#FFFFFF" };
    int m_defaultOpacity{ 100 };

    std::string m_colorOff{ "#FF0000" };
    std::string m_colorOn{ "#00FF00" };
    std::string m_colorInt{ "#FFFF00" };

    std::string m_outputPath{ "tmp.drawio" }; ///< The output file path for writing updated drawio xml.

    /// Hold the gui information for output links
    /** Output links aren't actual entities in basic instGraph, rather they are just pointers
     * from inputs to outputs.  But in the mxGraph XML they are entities that need to be managed
     * wo we need to collect them.
     */
    std::map<std::string, std::shared_ptr<guiData>> m_outputLinks;

  public:
    /// Default c'tor
    instGraphXML();

    /// Desctructor
    ~instGraphXML();

    int parseXMLDoc( std::string &emsg, const pugi::xml_document &doc );

    int loadXMLFile( std::string &emsg, const std::string &fname );

    /// Get the output file path for writing updated drawio xml
    /**
     * \returns a const reference to m_outputPath
     */
    const std::string &outputPath();

    /// Set the output file path for writing updated drawio xml
    void outputPath( const std::string &op /**< [in] the new output path */ );

    virtual void stateChange();

    virtual void valuePut( const std::string &node, const std::string &put, const ioDir &dir, const std::string &val );

    virtual void hideLinks();

    virtual void hidePuts();

}; // class instGraphXML

}; // namespace ingr

#endif // instGraphXML_hpp
