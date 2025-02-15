
#include <iostream>

#include "toml++/toml.h"

#include "instGraphTOML.hpp"

#if 0
    #define BREADCRUMB std::cerr << __FILE__ << " " << __LINE__ << std::endl;
#else
    #define BREADCRUMB
#endif

namespace ingr
{

instGraphTOML::instGraphTOML()
{
}

instGraphTOML::~instGraphTOML()
{
}

int instGraphTOML::parseTOMLTable( toml::table &tbl )
{
    toml::array &configNodes = *tbl["nodes"].as_array();

    for( auto &&tab : configNodes )
    {
        toml::node_view<toml::node> name = tab.as_table()->at_path( "name" );

        if( !name.is_string() )
        {
            if( tab.source().begin.operator bool() )
            {
                std::cerr << "Node at " << tab.source().begin << " has no name. Ignoring. \n";
            }
            else
            {
                std::cerr << "node without name\n";
            }
            continue;
        }

        std::string newname = tab.as_table()->at_path( "name" ).as_string()->get();

        std::cout << "Creating node: " << newname << "\n";

        BREADCRUMB

        instNode *newNode = new instNode( newname );

        std::pair<nodeMapT::iterator, bool> nodeRes = m_nodes.emplace( newname, newNode );

        BREADCRUMB

        toml::array *configOutputs = tab.at_path( "outputs" ).as_array();
        if( configOutputs == nullptr )
        {
            std::cerr << "   Node at " << tab.source().begin << " has no outputs.\n";
        }
        else
        {
            std::cout << "\tWith " << configOutputs->size() << " outputs\n";

            for( auto &&output : *configOutputs )
            {
                if( !output.as_table()->at_path( "name" ).is_string() )
                {
                    /// \todo test me
                    std::cerr << "\tOutput at " << output.source().begin << " has no name.";
                    return -1;
                }

                if( !output.as_table()->at_path( "beam" ).is_string() )
                {
                    /// \todo test me
                    std::cerr << "\tOutput at " << output.source().begin << " has no beam.";
                    return -1;
                }

                BREADCRUMB

                // First handle the beam, creating if needed or updating otherwise
                std::string beamName = output.as_table()->at_path( "beam" ).as_string()->get();

                ingr::instBeam *newBeam = nullptr;

                if( m_beams.count( beamName ) == 0 )
                {
                    newBeam = new instBeam;
                    std::pair<beamMapT::iterator, bool> beamRes = m_beams.emplace( beamName, newBeam );
                    newBeam->name( beamName );
                    std::cerr << "\tCreated beam " << beamName << "\n";
                }
                else
                {
                    newBeam = m_beams[beamName];

                    if( newBeam != nullptr )
                    {
                        if( newBeam->sourceValid() )
                        {
                            ///\todo test me
                            std::cerr << "\tBeam " << beamName << "("
                                      << output.as_table()->at_path( "beam" ).node()->source().begin
                                      << ") already has a source.\n";
                            return -1;
                        }
                    }

                    std::cerr << "\tUpdating beam " << beamName << "\n";
                }

                BREADCRUMB

                // Now handle the output
                std::string putName = output.as_table()->at_path( "name" ).as_string()->get();

                BREADCRUMB

                // Here we add the output
                instIOPut *newPut =
                    new instIOPut( { newNode, ingr::ioDir::output, putName, ingr::putType::light, newBeam } );
                newNode->addIOPut( newPut );
            }
        }

        toml::array *configInputs = tab.at_path( "inputs" ).as_array();
        if( configInputs == nullptr )
        {
            std::cout << "\tWith " << 0 << " inputs\n";
        }
        else
        {
            std::cout << "\tWith " << configInputs->size() << " inputs\n";

            for( auto &&input : *configInputs )
            {

                if( !input.as_table()->at_path( "name" ).is_string() )
                {
                    /// \todo test me
                    std::cerr << "Input at " << input.source().begin << " has no name.";
                    return -1;
                }

                if( !input.as_table()->at_path( "beam" ).is_string() )
                {
                    /// \todo test me
                    std::cerr << "Input at " << input.source().begin << " has no beam.";
                    return -1;
                }

                BREADCRUMB

                // First handle the beam, creating if needed or updating otherwise
                std::string beamName = input.as_table()->at_path( "beam" ).as_string()->get();

                instBeam *newBeam = nullptr;

                if( m_beams.count( beamName ) == 0 )
                {
                    newBeam = new instBeam;
                    std::pair<beamMapT::iterator, bool> beamRes = m_beams.emplace( beamName, newBeam );
                    std::cerr << "\tCreated beam " << beamName << "\n";
                    newBeam->name( beamName );
                }
                else
                {
                    newBeam = m_beams[beamName];
                    if( newBeam != nullptr )
                    {
                        if( newBeam->destValid() )
                        {
                            ///\todo test me
                            std::cerr << "Beam " << beamName << "("
                                      << input.as_table()->at_path( "beam" ).node()->source().begin
                                      << ") already has a dest.\n";
                            return -1;
                        }
                    }

                    std::cerr << "\tUpdating beam " << beamName << "\n";
                }

                BREADCRUMB

                // Now handle the input
                std::string putName = input.as_table()->at_path( "name" ).as_string()->get();

                BREADCRUMB

                instIOPut *newPut =
                    new instIOPut( { newNode, ingr::ioDir::input, putName, ingr::putType::light, newBeam } );
                std::string nnKey = newNode->addIOPut( newPut );

                if( input.as_table()->at_path( "outputLinks" ).is_array() )
                {
                    /// \todo test me
                    std::cerr << "\t\thas outputLinks:\n";
                    for( int n = 0; n < input.as_table()->at_path( "outputLinks" ).as_array()->size(); ++n )
                    {
                        std::string olink =
                            input.as_table()->at_path( "outputLinks" ).as_array()->at( n ).as_string()->get();
                        std::cerr << "\t\t    " << olink << "\n";

                        newNode->input( nnKey )->outputLink( olink );
                    }
                }
            }
        }
        // End of adding node
    }

    return 0;
}

int instGraphTOML::loadTOMLFile( const std::string &fname )
{
    toml::table tbl;
    try
    {
        tbl = toml::parse_file( fname );
        return parseTOMLTable( tbl );
    }
    catch( const toml::parse_error &err )
    {
        ///\todo identify errors
        std::cerr << "Parsing failed:\n" << err << "\n";
        return -1;
    }
}

} // namespace ingr
