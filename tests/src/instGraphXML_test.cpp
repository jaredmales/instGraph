/** \file instGraphXML_test.cpp
 * \brief Catch2 tests for instrument graph XML serialization.
 *
 * \ingroup instGraphXML_unit_tests
 */

#include "../catch2/catch.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../../src/instGraphXML.hpp"

/// \cond DOXYGEN_SUPPRESS_TEST_HARNESS
namespace
{
// Own an isolated source and output directory for one test.
struct temporaryDirectory
{
    std::filesystem::path path;

    temporaryDirectory()
    {
        char name[] = "/tmp/instGraphXML_test_XXXXXX";
        char *dir = ::mkdtemp( name );
        if( dir == nullptr )
        {
            throw std::runtime_error( "could not create test directory" );
        }
        path = dir;
    }

    ~temporaryDirectory()
    {
        std::error_code ec;
        std::filesystem::remove_all( path, ec );
    }
};

// Write a node and put whose value can be changed by valuePut().
void writeGraph( const std::filesystem::path &path )
{
    std::ofstream out( path );
    out << "<mxfile><diagram><mxGraphModel><root>"
           "<mxCell id=\"0\"/><mxCell id=\"1\" parent=\"0\"/>"
           "<mxCell id=\"node:test\" value=\"test\" style=\"strokeColor=#FF0000;\"/>"
           "<mxCell id=\"input:test:in\" value=\"before\" style=\"strokeColor=#FF0000;\"/>"
           "</root></mxGraphModel></diagram></mxfile>";
}
} // namespace
/// \endcond

namespace unitTest::instGraphXMLTest
{

/// Disabled automatic saves retain mutations for explicit serialization
/**
 * \ingroup instGraphXML_unit_tests
 */
TEST_CASE( "Disabled automatic saves retain mutations for explicit serialization", "[instGraphXML]" )
{
    temporaryDirectory temp;
    auto input = temp.path / "source.drawio";
    auto output = temp.path / "output.drawio";
    writeGraph( input );

    ingr::instGraphXML graph;
    std::string error;
    REQUIRE( graph.loadXMLFile( error, input.string() ) == 0 );
    graph.outputPath( output.string() );
    graph.autoSave( false );
    graph.valuePut( "test", "in", ingr::ioDir::input, "after" );
    graph.stateChange();
    REQUIRE_FALSE( std::filesystem::exists( output ) );

    std::string xml;
    REQUIRE( graph.serializeXML( xml, error ) == 0 );
    REQUIRE( error.empty() );
    REQUIRE( xml.find( "value=\"after\"" ) != std::string::npos );
    REQUIRE_FALSE( std::filesystem::exists( output ) );
}

/// Automatic saves report a failed file write
/**
 * \ingroup instGraphXML_unit_tests
 */
TEST_CASE( "Automatic saves report a failed file write", "[instGraphXML]" )
{
    temporaryDirectory temp;
    auto input = temp.path / "source.drawio";
    writeGraph( input );

    ingr::instGraphXML graph;
    std::string error;
    REQUIRE( graph.loadXMLFile( error, input.string() ) == 0 );
    graph.outputPath( ( temp.path / "missing" / "output.drawio" ).string() );
    REQUIRE_THROWS_AS( graph.valuePut( "test", "in", ingr::ioDir::input, "after" ), std::runtime_error );
    REQUIRE_THROWS_AS( graph.stateChange(), std::runtime_error );
}

} // namespace unitTest::instGraphXMLTest
