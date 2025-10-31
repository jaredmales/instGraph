#include <iostream>
#include <vector>

#include "instGraphXML.hpp"

#define PUGIXML_HEADER_ONLY
#include "pugixml/pugixml.hpp"

namespace ingr
{

#define MXGPARSE_ERR_NODE_NTL ( -20 )
#define MXGPARSE_ERR_NODE_NNF ( -25 )
#define MXGPARSE_ERR_NODE_NON ( -30 )

int parseNode( std::string &name, std::string &emsg, const std::string &value, const size_t &fc )
{
    name = "";
    emsg = "";

    if( fc > 4 )
    {
        emsg = "mxCell id starts with 'n' but node is not found . (id=\"" + value + "\")";
        return MXGPARSE_ERR_NODE_NTL;
    }
    else if( fc == 4 )
    {
        if( value.substr( 0, 4 ) != "node" )
        {
            emsg = "mxCell id starts with 'n' but node is not found . (id=\"" + value + "\")";
            return MXGPARSE_ERR_NODE_NNF;
        }
    }
    else if( fc != 1 )
    {
        emsg = "mxCell id starts with 'n' but node is not found . (id=\"" + value + "\")";
        return MXGPARSE_ERR_NODE_NON;
    }

    name = value.substr( fc + 1 );

    return 0;
}

#define MXGPARSE_ERR_PUT_INPVALNL ( -55 )
#define MXGPARSE_ERR_PUT_INPNOF ( -60 )
#define MXGPARSE_ERR_PUT_IVALNL ( -65 )
#define MXGPARSE_ERR_PUT_IPE ( -70 )
#define MXGPARSE_ERR_PUT_OUTVALNL ( -75 )
#define MXGPARSE_ERR_PUT_OUTNOF ( -80 )
#define MXGPARSE_ERR_PUT_OVALNL ( -85 )
#define MXGPARSE_ERR_PUT_OPE ( -90 )
#define MXGPARSE_ERR_PUT_LIGHTNF ( -95 )
#define MXGPARSE_ERR_PUT_LNF ( -100 )
#define MXGPARSE_ERR_PUT_DATANF ( -105 )
#define MXGPARSE_ERR_PUT_DNF ( -110 )
#define MXGPARSE_ERR_PUT_POWERNF ( -115 )
#define MXGPARSE_ERR_PUT_PNF ( -120 )
#define MXGPARSE_ERR_PUT_MECHNF ( -125 )
#define MXGPARSE_ERR_PUT_MNF ( -130 )
#define MXGPARSE_ERR_PUT_FLUIDNF ( -135 )
#define MXGPARSE_ERR_PUT_FNF ( -140 )
#define MXGPARSE_ERR_PUT_TINV ( -145 )
#define MXGPARSE_ERR_PUT_NO2C ( -150 )
#define MXGPARSE_ERR_PUT_NONN ( -155 )
#define MXGPARSE_ERR_PUT_NPN ( -160 )

int parsePut( ioDir &dir,
              putType &type,
              std::string &node,
              std::string &name,
              std::string &emsg,
              const std::string &value,
              const size_t &fc )
{
    emsg = "";

    // First check for put type
    size_t tfc = fc;
    size_t tc = std::string::npos; // type marker
    if( fc > 1 )
    {
        tc = value.rfind( fc, '.' );
        if( tc != std::string::npos )
        {
            tfc = tc;
        }
    }
    // here if tc != npos then there is a type

    // Get direction (input or output)
    if( value[0] == 'i' )
    {
        if( tfc == 5 )
        {
            if( value.length() < 9 ) // validate that there is at least 'input:n:n'
            {
                emsg = "mxCell input id value isn't long enough, must be at least input:n:n (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_INPVALNL;
            }

            if( value.substr( 0, 5 ) != "input" )
            {
                emsg = "mxCell input id value parse error (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_INPNOF;
            }
        }
        else if( tfc == 1 )
        {
            if( value.length() < 5 ) // validate that there is at least 'i:n:n'
            {
                emsg = "mxCell input id value isn't long enough, must be at east i:n:n (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_IVALNL;
            }
        }
        else
        {
            emsg = "mxCell input id value parse error (id=\"" + value + "\")";
            return MXGPARSE_ERR_PUT_IPE; // it's neither "input" nor "i"
        }

        dir = ioDir::input;
    }
    else if( value[0] == 'o' )
    {
        if( tfc == 6 )
        {
            if( value.length() < 10 ) // validate that there is at least 'output:n:n'
            {
                emsg = "mxCell output id value isn't long enough, must be at least output:n:n (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_OUTVALNL;
            }

            if( value.substr( 0, 6 ) != "output" )
            {
                emsg = "mxCell output id value parse error (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_OUTNOF;
            }
        }
        else if( tfc == 1 )
        {
            if( value.length() < 5 ) // validate that there is at least 'i:n:n'
            {
                emsg = "mxCell input id value isn't long enough, must be at least o:n:n (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_OVALNL;
            }
        }
        else
        {
            emsg = "mxCell output id value parse error (id=\"" + value + "\")";
            return MXGPARSE_ERR_PUT_OPE; // it's neither "output" nor "o"
        }

        dir = ioDir::output;
    }

    if( tc != std::string::npos )
    {
        // Now check for l, d, p, m, f

        tc += 1;
        if( value[tc] == 'l' )
        {
            if( fc - tc == 5 )
            {
                if( value.substr( tc, 5 ) != "light" )
                {
                    emsg = "mxCell id 'type' parse error, light not found (id=\"" + value + "\")";
                    return MXGPARSE_ERR_PUT_LIGHTNF;
                }
            }
            else if( fc - tc != 1 )
            {
                emsg = "mxCell id 'type' parse error, light not found (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_LNF;
            }

            type = putType::light;
        }
        else if( value[tc] == 'd' )
        {
            if( fc - tc == 4 )
            {
                if( value.substr( tc, 4 ) != "data" )
                {
                    emsg = "mxCell id 'type' parse error, data not found (id=\"" + value + "\")";
                    return MXGPARSE_ERR_PUT_DATANF;
                }
            }
            else if( fc - tc != 1 )
            {
                emsg = "mxCell id 'type' parse error, data not found (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_DNF;
            }

            type = putType::data;
        }
        else if( value[tc] == 'p' )
        {
            if( fc - tc == 5 )
            {
                if( value.substr( tc, 4 ) != "power" )
                {
                    emsg = "mxCell id 'type' parse error, power not found (id=\"" + value + "\")";
                    return MXGPARSE_ERR_PUT_POWERNF;
                }
            }
            else if( fc - tc != 1 )
            {
                emsg = "mxCell id 'type' parse error, power not found (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_PNF;
            }

            type = putType::power;
        }
        else if( value[tc] == 'm' )
        {
            if( fc - tc == 10 )
            {
                if( value.substr( tc, 4 ) != "mechanical" )
                {
                    emsg = "mxCell id 'type' parse error, mechanical not found (id=\"" + value + "\")";
                    return MXGPARSE_ERR_PUT_MECHNF;
                }
            }
            else if( fc - tc != 1 )
            {
                emsg = "mxCell id 'type' parse error, mechanical not found (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_MNF;
            }

            type = putType::mechanical;
        }
        else if( value[tc] == 'f' )
        {
            if( fc - tc == 5 )
            {
                if( value.substr( tc, 4 ) != "fluid" )
                {
                    emsg = "mxCell id 'type' parse error, fluid not found (id=\"" + value + "\")";
                    return MXGPARSE_ERR_PUT_FLUIDNF;
                }
            }
            else if( fc - tc != 1 )
            {
                emsg = "mxCell id 'type' parse error, fluid not found (id=\"" + value + "\")";
                return MXGPARSE_ERR_PUT_FNF;
            }

            type = putType::fluid;
        }
        else
        {
            emsg = "mxCell id value 'type' is not valid (id=\"" + value + "\")";
            return MXGPARSE_ERR_PUT_TINV;
        }
    }
    else
    {
        type = putType::light;
    }

    size_t sc = value.find( ':', fc + 1 );
    if( sc == std::string::npos )
    {
        emsg = "mxCell ioput without second ':' (id=\"" + value + "\")";
        return MXGPARSE_ERR_PUT_NO2C;
    }

    if( sc == fc + 1 )
    {
        emsg = "mxCell ioput without node name (id=\"" + value + "\")";
        return MXGPARSE_ERR_PUT_NONN;
    }

    if( sc == value.length() - 1 )
    {
        emsg = "mxCell ioput without ioput name (id=\"" + value + "\")";
        return MXGPARSE_ERR_PUT_NPN;
    }

    node = value.substr( fc + 1, sc - ( fc + 1 ) );
    name = value.substr( sc + 1 );

    return 0;
}

#define MXGPARSE_ERR_BEAM_NOBEAM ( -200 )
#define MXGPARSE_ERR_BEAM_NOB ( -205 )
#define MXGPARSE_ERR_BEAM_NON ( -210 )
#define MXGPARSE_ERR_BEAM_NOS ( -215 )
#define MXGPARSE_ERR_BEAM_NOO ( -220 )
#define MXGPARSE_ERR_BEAM_NOOC ( -225 )
#define MXGPARSE_ERR_BEAM_OPE ( -2000 )
#define MXGPARSE_ERR_BEAM_ODIR ( -230 )

#define MXGPARSE_ERR_BEAM_NOT ( -235 )
#define MXGPARSE_ERR_BEAM_NOI ( -240 )
#define MXGPARSE_ERR_BEAM_NOIC ( -245 )
#define MXGPARSE_ERR_BEAM_IPE ( -3000 )
#define MXGPARSE_ERR_BEAM_IDIR ( -250 )

#define MXGPARSE_ERR_BEAM_TYPES ( -255 )

int parseBeam( std::string &name,
               std::string &outNode,
               std::string &outName,
               std::string &inNode,
               std::string &inName,
               std::string &emsg,
               const std::string &value,
               const size_t &fc,
               pugi::xml_attribute &source,
               pugi::xml_attribute &target,
               const std::string &beamStr = "beam" )
{
    if( fc == beamStr.size() )
    {
        if( value.substr( 0, beamStr.size() ) != beamStr )
        {
            emsg = "mxCell id " + beamStr + " not found (id=\"" + value + "\")";
            return MXGPARSE_ERR_BEAM_NOBEAM;
        }
    }
    else if( fc != 1 )
    {
        emsg = "mxCell id " + beamStr + " not found (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NOB;
    }

    name = value.substr( fc + 1 );

    if( name.size() == 0 )
    {
        emsg = "mxCell id " + beamStr + " name not found (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NON;
    }

    if( source.empty() )
    {
        emsg = "mxCell id " + beamStr + " sourcenot found (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NOS;
    }

    ioDir odir;
    putType otype;
    std::string pemsg;
    std::string pvalue = source.value();

    if( pvalue[0] != 'o' )
    {
        emsg = "mxCell id " + beamStr + " output not found in source (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NOO;
    }

    size_t pfc = pvalue.find( ":" );

    if( pfc == std::string::npos )
    {
        emsg = "mxCell id " + beamStr + " output not found in source (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NOOC;
    }

    int pec = parsePut( odir, otype, outNode, outName, pemsg, pvalue, pfc );

    if( pec < 0 )
    {
        emsg = beamStr + " output parse error: " + pemsg;
        return MXGPARSE_ERR_BEAM_OPE + pec;
    }

    if( odir != ioDir::output )
    {
        emsg = beamStr + " source is not an output (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_ODIR;
    }

    if( target.empty() )
    {
        emsg = "mxCell id " + beamStr + " target not found (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NOT;
    }

    ioDir idir;
    putType itype;
    pvalue = target.value();

    if( pvalue[0] != 'i' )
    {
        emsg = "mxCell id " + beamStr + " input not found in target (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NOI;
    }

    pfc = pvalue.find( ":" );

    if( pfc == std::string::npos )
    {
        emsg = "mxCell id " + beamStr + " input not found in target (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_NOIC;
    }

    pec = parsePut( idir, itype, inNode, inName, pemsg, pvalue, pfc );

    if( pec < 0 )
    {
        emsg = beamStr + " input parse error: " + pemsg;
        return MXGPARSE_ERR_BEAM_IPE + pec;
    }

    if( idir != ioDir::input )
    {
        emsg = beamStr + " target is not an input (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_IDIR;
    }

    if( otype != itype )
    {
        emsg = beamStr + " source and target type mismatch (id=\"" + value + "\")";
        return MXGPARSE_ERR_BEAM_TYPES;
    }

    return 0;
}

// walker struct for depth traversal to look for the mxGraphModel
struct find_mxGraph : pugi::xml_tree_walker
{
    pugi::xml_node mxGraph;
    virtual bool for_each( pugi::xml_node &node )
    {
        if( std::string( node.name() ) == "mxGraphModel" )
        {
            mxGraph = node;
            return false;
        }

        return true; // continue traversal
    }
};

instGraphXML::instGraphXML()
{
    m_doc = new pugi::xml_document;
}

instGraphXML::~instGraphXML()
{
    // Clean up aux data
    for( auto it : m_nodes )
    {
        if( it.second->auxDataValid() )
        {
            delete static_cast<auxDataT *>( it.second->auxData() );
            it.second->auxData( nullptr );
        }

        for( auto iit : it.second->inputs() )
        {
            if( iit.second->auxDataValid() )
            {
                delete static_cast<auxDataT *>( iit.second->auxData() );
                iit.second->auxData( nullptr );
            }
        }

        for( auto oit : it.second->outputs() )
        {
            if( oit.second->auxDataValid() )
            {
                delete static_cast<auxDataT *>( oit.second->auxData() );
                oit.second->auxData( nullptr );
            }
        }
    }

    for( auto it : m_beams )
    {
        if( it.second->auxDataValid() )
        {
            delete static_cast<auxDataT *>( it.second->auxData() );
            it.second->auxData( nullptr );
        }
    }

    if( m_doc )
    {
        delete m_doc;
    }
}

#define MXGPARSE_ERR_DOC_NOMXG ( -500 )
#define MXGPARSE_ERR_DOC_NOROOT ( -505 )
#define MXGPARSE_ERR_DOC_CE ( -510 )
#define MXGPARSE_ERR_DOC_SE ( -515 )
#define MXGPARSE_ERR_DOC_OLDN ( -520 )

struct egData
{
    std::string name;
    std::string type;
    std::string payload;
    pugi::xml_node *node {nullptr};
};

int instGraphXML::parseXMLDoc( std::string &emsg )
{
    std::vector<egData> extras;

    find_mxGraph mxGraph;
    m_doc->traverse( mxGraph );

    if( mxGraph.mxGraph.empty() )
    {
        emsg = "no mxGraphModel found in doc";
        return MXGPARSE_ERR_DOC_NOMXG;
    }

    pugi::xml_node root = mxGraph.mxGraph.child( "root" );

    if( root.empty() )
    {
        emsg = "no root found in mxGraphModel";
        return MXGPARSE_ERR_DOC_NOROOT;
    }

    for( pugi::xml_node cell : root.children( "mxCell" ) )
    {
        pugi::xml_attribute id = cell.attribute( "id" );
        if( id.name()[0] == '\0' )
        {
            continue;
        }

        std::string value = id.as_string( "" );
        if( value.length() < 1 )
        {
            emsg = "mxCell with empty id (length 0)";
            return MXGPARSE_ERR_DOC_CE;
        }

        size_t fc = value.find( ':' );
        if( fc == std::string::npos ) // not an instGraph cell
        {
            // This could just be id="0" or id="1", or some other entity of the graph
            continue;
        }

        if( fc == 0 || fc == value.length() ) // starts or ends with :
        {
            emsg = "mxCell id starts or ends with ':'. (id=\"" + value + "\")";
            return MXGPARSE_ERR_DOC_SE;
        }

        if( value[0] == 'n' )
        {
            std::string name;

            int ec = parseNode( name, emsg, value, fc );
            if( ec < 0 )
            {
                std::cerr << "Error parsing node " << name << ": " << emsg << " (" << ec << ")\n";

                return ec;
            }

            instNode *newNode = new instNode( name );

            std::pair<nodeMapT::iterator, bool> nodeRes = m_nodes.emplace( name, newNode );
            ///\todo result check

            guiData *gd = new guiData( cell );

            newNode->auxData( gd );
        }
        else if( (value[0] == 'o' && value[1] != 'f') || value[0] == 'i' )
        {
            ioDir dir;
            putType type;
            std::string node;
            std::string name;

            int ec = parsePut( dir, type, node, name, emsg, value, fc );
            if( ec < 0 )
            {
                return ec;
            }

            if( m_nodes.count( node ) == 0 ) // node might not exist b/c it hasn't been parsed yet
            {
                instNode *newNode = new instNode( node );

                std::pair<nodeMapT::iterator, bool> nodeRes = m_nodes.emplace( node, newNode );
            }

            instNode *newNode = m_nodes[node];

            instIOPut *newPut = new instIOPut( { newNode, dir, name, type, nullptr } );
            newNode->addIOPut( newPut );
            newPut->parentGraph( this ); ///\todo we need a single way to do this all

            // pugi::xml_node * xn = new pugi::xml_node(cell);
            guiData *gd = new guiData( cell );
            newPut->auxData( gd );
        }
        else if( value[0] == 'b' )
        {
            std::string name;
            std::string outNode;
            std::string outName;
            std::string inNode;
            std::string inName;

            pugi::xml_attribute source = cell.attribute( "source" );
            pugi::xml_attribute target = cell.attribute( "target" );

            int ec = parseBeam( name, outNode, outName, inNode, inName, emsg, value, fc, source, target );

            if( ec < 0 )
            {
                return ec;
            }

            instBeam *newBeam = new instBeam;
            std::pair<beamMapT::iterator, bool> beamRes = m_beams.emplace( name, newBeam );
            newBeam->name( name );
            newBeam->parentGraph( this ); ///\todo we need a single way to do this all

            if( m_nodes.count( outNode ) > 0 )
            {
                if( !m_nodes[outNode]->outputValid( outName ) )
                {
                    std::string msg = "node \"";
                    msg += outNode + "\"";
                    msg += " has no ouput \"";
                    msg += outName + "\"";
                    msg += " for beam id \"";
                    msg += value + "\".  Beams must be at end of drawio file!";
                    msg += " (ingr::instGraphXML::parseXMLDoc ";
                    msg += __FILE__;
                    msg += " ";
                    msg += std::to_string( __LINE__ );
                    msg += ")";
                    throw std::runtime_error( msg );
                }

                m_nodes[outNode]->output( outName )->beam( newBeam );
            }
            else
            {
                std::string msg = "node \"";
                msg += outNode + "\"";
                msg += " not found for beam id \"";
                msg += value + "\".  Beams must be at end of drawio file!";
                msg += " (ingr::instGraphXML::parseXMLDoc ";
                msg += __FILE__;
                msg += " ";
                msg += std::to_string( __LINE__ );
                msg += ")";

                throw std::runtime_error( msg );
            }

            newBeam->source( m_nodes[outNode]->output( outName ) );

            if( m_nodes.count( inNode ) > 0 )
            {
                if( !m_nodes[inNode]->inputValid( inName ) )
                {
                    std::string msg = "node \"";
                    msg += inNode + "\"";
                    msg += " has no input \"";
                    msg += inName + "\"";
                    msg += " for beam id \"";
                    msg += value + "\".  Beams must be at end of drawio file!";
                    msg += " (ingr::instGraphXML::parseXMLDoc ";
                    msg += __FILE__;
                    msg += " ";
                    msg += std::to_string( __LINE__ );
                    msg += ")";
                    throw std::runtime_error( msg );
                }

                m_nodes[inNode]->input( inName )->beam( newBeam );
            }
            else
            {
                std::string msg = "node \"";
                msg += inNode + "\"";
                msg += " not found for beam id \"";
                msg += value + "\".  Beams must be at end of drawio file!";
                msg += " (ingr::instGraphXML::parseXMLDoc ";
                msg += __FILE__;
                msg += " ";
                msg += std::to_string( __LINE__ );
                msg += ")";

                throw std::runtime_error( msg );
            }

            newBeam->dest( m_nodes[inNode]->input( inName ) );

            guiData *gd = new guiData( cell );
            // pugi::xml_node * xn = new pugi::xml_node(cell);
            newBeam->auxData( gd );
        }
        else if( value[0] == 'l' ) //a link
        {
            std::string name;
            std::string outNode;
            std::string outName;
            std::string inNode;
            std::string inName;

            pugi::xml_attribute source = cell.attribute( "source" );
            pugi::xml_attribute target = cell.attribute( "target" );

            // note that when calling for a link instead of a beam we swap target and source
            int ec = parseBeam( name, outNode, outName, inNode, inName, emsg, value, fc, target, source, "link" );

            if( ec < 0 )
            {
                return ec;
            }

            if( outNode != inNode )
            {
                emsg = "output link has different nodes for source and target ':'. (id=\"" + value + "\")";
                return MXGPARSE_ERR_DOC_OLDN;
            }
            m_nodes[inNode]->input( inName )->outputLink( outName );

            m_outputLinks.insert( std::pair( value, std::make_shared<guiData>( cell ) ) );
        }
        else if( value[0] == 'o' && value[1] == 'f' ) //an off link
        {
            std::string name;
            std::string outNode;
            std::string outName;
            std::string inNode;
            std::string inName;

            pugi::xml_attribute source = cell.attribute( "source" );
            pugi::xml_attribute target = cell.attribute( "target" );

            // note that when calling for a link instead of a beam we swap target and source
            int ec = parseBeam( name, outNode, outName, inNode, inName, emsg, value, fc, target, source, "offlink" );

            if( ec < 0 )
            {
                return ec;
            }

            if( outNode != inNode )
            {
                emsg = "output off link has different nodes for source and target ':'. (id=\"" + value + "\")";
                return MXGPARSE_ERR_DOC_OLDN;
            }
            m_nodes[inNode]->input( inName )->outputOffLink( outName );

            m_outputOffLinks.insert( std::pair( value, std::make_shared<guiData>( cell ) ) );
        }
        else if( fc != value.size() - 1 )
        {
            std::string type = value.substr( 0, fc );

            size_t ec = value.find( ':', fc + 1 );
            if( ec == std::string::npos )
            {
                ec = value.size();
            }

            std::string name = value.substr( fc + 1, ec - ( fc + 1 ) );

            std::string payload;
            if( ec < value.size() )
            {
                payload = value.substr( ec + 1, value.size() - ( fc + 1 ) );
            }

            extras.emplace_back();
            extras.back().name = name;
            extras.back().type = type;
            extras.back().payload = payload;
            extras.back().node = new pugi::xml_node(cell); //Construct

        }
    }

    for( auto &node : m_nodes )
    {
        if( !node.second->auxDataValid() )
        {
            std::string msg = "instGraphXML::parseXMLDoc: Node ";
            msg += node.second->name() + " was not found in the XML but it was referred to.";
            throw std::runtime_error( msg );
        }
    }

    for( auto &&nn : m_nodes )
    {
        nn.second->updateOutputLinks();
    }

    for( auto &extra : extras )
    {
        if( m_nodes.count( extra.name ) > 0 )
        {
            //std::cerr << "Found extra: " << extra.type << " for " << extra.name << ": " << extra.payload << "\n";

            if( !m_nodes[extra.name]->auxDataValid() )
            {
                std::cerr << "no valid auxData for " << extra.name << "\n";
                continue;
            }
            guiData *gd = static_cast<guiData *>( m_nodes[extra.name]->auxData() );

            extraGuiData egd; // = new extraGuiData;
            egd.payload = extra.payload;
            egd.gdata = std::make_shared<guiData>( extra.node );

            gd->extraData.insert( { extra.type, egd } );
        }
        else
        {
            std::cerr << "No node for extra: " << extra.type << " for " << extra.name << " " << extra.payload << "\n";

            delete extra.node;
        }
    }
    return 0;
}

int instGraphXML::parseXMLDoc( std::string &emsg, const pugi::xml_document &doc )
{
    m_doc->reset( doc );

    return parseXMLDoc( emsg );
}

int instGraphXML::loadXMLFile( std::string &emsg, const std::string &fname )
{
    if( !m_doc->load_file( fname.c_str() ) )
    {
        emsg = "error loading file " + fname;
        return -1;
    }

    return parseXMLDoc( emsg );
}

const std::string &instGraphXML::outputPath()
{
    return m_outputPath;
}

void instGraphXML::outputPath( const std::string &op )
{
    m_outputPath = op;
}

void instGraphXML::stateChange()
{
    for( auto it : m_beams )
    {
        if( it.second->auxDataValid() )
        {
            auxDataT *auxData = static_cast<auxDataT *>( it.second->auxData() );
            if( it.second->state() == beamState::on )
            {
                auxData->strokeColor( m_colorOn );
                auxData->fontColor( m_colorOn );
            }
            else if( it.second->state() == beamState::intermediate )
            {
                auxData->strokeColor( m_colorInt );
                auxData->fontColor( m_colorInt );
            }
            else if( it.second->state() == beamState::off )
            {
                auxData->strokeColor( m_colorOff );
                auxData->fontColor( m_colorOff );
            }
        }
    }

    for( auto it : m_nodes )
    {
        for( auto iit : it.second->inputs() )
        {
            if( iit.second->auxDataValid() )
            {
                auxDataT *auxData = static_cast<auxDataT *>( iit.second->auxData() );

                if( iit.second->state() == putState::on )
                {
                    auxData->strokeColor( m_colorOn );
                    auxData->fontColor( m_colorOn );
                }
                else if( iit.second->state() == putState::waiting )
                {
                    auxData->strokeColor( m_colorInt );
                    auxData->fontColor( m_colorInt );
                }
                else if( iit.second->state() == putState::off )
                {
                    auxData->strokeColor( m_colorOff );
                    auxData->fontColor( m_colorOff );
                }
            }
        }

        for( auto oit : it.second->outputs() )
        {
            if( oit.second->auxDataValid() )
            {
                auxDataT *auxData = static_cast<auxDataT *>( oit.second->auxData() );

                if( oit.second->state() == putState::on )
                {
                    auxData->strokeColor( m_colorOn );
                    auxData->fontColor( m_colorOn );
                }
                else if( oit.second->state() == putState::waiting )
                {
                    auxData->strokeColor( m_colorInt );
                    auxData->fontColor( m_colorInt );
                }
                else if( oit.second->state() == putState::off )
                {
                    auxData->strokeColor( m_colorOff );
                    auxData->fontColor( m_colorOff );
                }
            }
        }
    }

    m_doc->save_file( m_outputPath.c_str() );
}

///\todo make this use ioDIR
void instGraphXML::valuePut( const std::string &node, const std::string &put, const ioDir &dir, const std::string &val )
{
    if( !m_nodes.count( node ) == 1 )
    {
        return;
    }

    instNode *nptr;

    try
    {
        nptr = instGraph::node( node );
    }
    catch( ... )
    {
        return;
    }

    instIOPut *pptr;
    try
    {
        if( dir == ioDir::input )
        {
            pptr = nptr->input( put );
        }
        else
        {
            pptr = nptr->output( put );
        }
    }
    catch( ... )
    {
        return;
    }

    if( !pptr->auxDataValid() )
    {
        return;
    }

    static_cast<auxDataT *>( pptr->auxData() )->value( val );

    m_doc->save_file( m_outputPath.c_str() );
}

void instGraphXML::valueExtra( const std::string &node, const std::string &extra, const std::string &val )
{
    if( !m_nodes.count( node ) == 1 )
    {
        return;
    }

    instNode *nptr;

    try
    {
        nptr = instGraph::node( node );
    }
    catch( ... )
    {
        return;
    }

    if( !nptr->auxDataValid() )
    {
        return;
    }

    auxDataT * ad = static_cast<auxDataT *>( nptr->auxData() );

    if(ad->extraData.count(extra) == 0)
    {
        return;
    }

    auto edL = ad->extraData.lower_bound(extra);
    auto edU = ad->extraData.upper_bound(extra);
    while(edL != edU)
    {
        edL->second.gdata->value( val );
        ++edL;
    }

    m_doc->save_file( m_outputPath.c_str() );
}

void instGraphXML::hideLinks()
{
    for( auto &lit : m_outputLinks )
    {
        lit.second->opacity( 0 );
    }

    for( auto &lit : m_outputOffLinks )
    {
        lit.second->opacity( 0 );
    }
}

void instGraphXML::hidePuts()
{
    for( auto &nit : m_nodes )
    {
        for( auto &pit : nit.second->inputs() )
        {
            if( pit.second->auxDataValid() )
            {
                guiData *gd = static_cast<guiData *>( pit.second->auxData() );
                gd->opacity( 0 );
                gd->textOpacity( 0 );
            }
        }
    }

    for( auto &nit : m_nodes )
    {
        for( auto &pit : nit.second->outputs() )
        {
            if( pit.second->auxDataValid() )
            {
                guiData *gd = static_cast<guiData *>( pit.second->auxData() );
                gd->opacity( 0 );
                gd->textOpacity( 0 );
            }
        }
    }
}

instGraphXML::guiData::guiData( pugi::xml_node *xn )
{
    xmlNode = xn;
    findColors();
}

instGraphXML::guiData::guiData( const pugi::xml_node &xn )
{
    xmlNode = new pugi::xml_node( xn );
    findColors();
}

instGraphXML::guiData::~guiData()
{
    if( xmlNode != nullptr )
    {
        delete xmlNode;
    }
}

void instGraphXML::guiData::findColors()
{
    if( xmlNode == nullptr )
    {
        styleValue = "";
        strokeColorPos.clear();
        fontColorPos.clear();
        opacityPos.clear();
        textOpacityPos.clear();
        return;
    }

    pugi::xml_attribute style = xmlNode->attribute( "style" );

    if( !style.empty() )
    {
        styleValue = style.value();

        strokeColorPos.findKey( styleValue, "strokeColor" );
        fontColorPos.findKey( styleValue, "fontColor" );
        opacityPos.findKey( styleValue, "opacity" );
        textOpacityPos.findKey( styleValue, "textOpacity" );
    }
    else
    {
        styleValue = "";
        strokeColorPos.clear();
        fontColorPos.clear();
        opacityPos.clear();
        textOpacityPos.clear();
    }
}

void instGraphXML::guiData::strokeColor( const std::string &color )
{
    if( xmlNode == nullptr )
    {
        std::string msg = "instGraphXML::guiData::strokeColor: xmlNode null";
        throw std::runtime_error( msg );
    }

    pugi::xml_attribute style = xmlNode->attribute( "style" );

    if( style.empty() )
    {
        std::string msg = "instGraphXML::guiData::strokeColor: no `style` attribute";
        throw std::runtime_error( msg );
    }

    // Add it if it doesn't exist
    if( strokeColorPos.keyPos == std::string::npos )
    {
        styleValue += std::string("strokeColor=") + m_defaultColor + ";";
        style.set_value( styleValue.c_str() );
        findColors();

        if( strokeColorPos.keyPos == std::string::npos )
        {
            std::string msg = "Unable to add strokeColor to style attribute";
            throw std::runtime_error( msg );
        }
    }

    bool szch = strokeColorPos.setValue( styleValue, color );

    style.set_value( styleValue.c_str() );

    if( szch )
    {
        return findColors();
    }
}

void instGraphXML::guiData::fontColor( const std::string &color )
{
    if( xmlNode == nullptr )
    {
        std::string msg = "instGraphXML::guiData::fontColor: xmlNode null";
        throw std::runtime_error( msg );
    }

    pugi::xml_attribute style = xmlNode->attribute( "style" );

    if( style.empty() )
    {
        std::string msg = "instGraphXML::guiData::fontColor: no `style` attribute";
        throw std::runtime_error( msg );
    }

    // Add it if it doesn't exist
    if( fontColorPos.keyPos == std::string::npos )
    {
        styleValue += std::string("fontColor=") + m_defaultColor + ";";
        style.set_value( styleValue.c_str() );
        findColors();

        if( fontColorPos.keyPos == std::string::npos )
        {
            std::string msg = "Unable to add fontColor to style attribute";
            throw std::runtime_error( msg );
        }
    }

    bool szch = fontColorPos.setValue( styleValue, color );

    style.set_value( styleValue.c_str() );

    if( szch )
    {
        return findColors();
    }
}

void instGraphXML::guiData::opacity( int op )
{
    if( xmlNode == nullptr )
    {
        std::string msg = "instGraphXML::guiData::opacity: xmlNode null";
        throw std::runtime_error( msg );
    }

    pugi::xml_attribute style = xmlNode->attribute( "style" );

    if( style.empty() )
    {
        std::string msg = "instGraphXML::guiData::opacity: no `style` attribute";
        throw std::runtime_error( msg );
    }

    // Add it if it doesn't exist
    if( opacityPos.keyPos == std::string::npos )
    {
        styleValue += "opacity=" + std::to_string( m_defaultOpacity ) + ";";
        style.set_value( styleValue.c_str() );
        findColors();

        if( opacityPos.keyPos == std::string::npos )
        {
            std::string msg = "Unable to add opacity to style attribute";
            throw std::runtime_error( msg );
        }
    }

    bool szch = opacityPos.setValue( styleValue, std::to_string( op ) );

    style.set_value( styleValue.c_str() );

    if( szch )
    {
        return findColors();
    }
}

void instGraphXML::guiData::textOpacity( int op )
{
    if( xmlNode == nullptr )
    {
        std::string msg = "instGraphXML::guiData::textOpacity: xmlNode null";
        throw std::runtime_error( msg );
    }

    pugi::xml_attribute style = xmlNode->attribute( "style" );

    if( style.empty() )
    {
        std::string msg = "instGraphXML::guiData::textOpacity: no `style` attribute";
        throw std::runtime_error( msg );
    }

    // Add it if it doesn't exist
    if( textOpacityPos.keyPos == std::string::npos )
    {
        styleValue += "textOpacity=" + std::to_string( m_defaultOpacity ) + ";";
        style.set_value( styleValue.c_str() );
        findColors();

        if( textOpacityPos.keyPos == std::string::npos )
        {
            std::string msg = "Unable to add opacity to style attribute";
            throw std::runtime_error( msg );
        }
    }

    bool szch = textOpacityPos.setValue( styleValue, std::to_string( op ) );

    style.set_value( styleValue.c_str() );

    if( szch )
    {
        return findColors();
    }
}

void instGraphXML::guiData::value( const std::string &val )
{
    if( xmlNode == nullptr )
    {
        return;
    }

    pugi::xml_attribute value = xmlNode->attribute( "value" );

    if( !value.empty() )
    {
        value.set_value( val.c_str() );
    }
}

} // namespace ingr
