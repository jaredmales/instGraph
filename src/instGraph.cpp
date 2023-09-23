
#include <iostream>

#include "toml++/toml.h"

#include "instGraph.hpp"

#if 0
   #define BREADCRUMB std::cerr << __FILE__ << " " << __LINE__ << std::endl;
#else
   #define BREADCRUMB 
#endif

namespace ingr
{

instGraph::instGraph()
{
}

instGraph::~instGraph()
{
    for(auto && beam : m_beams)
    {
        delete beam.second;
    }

    for(auto && node : m_nodes)
    {
        delete node.second;
    }
}

const instGraph::nodeMapT & instGraph::nodes()
{
    return m_nodes;
}

instNode * instGraph::node( const std::string & key )
{
    if(m_nodes.count(key) != 1)
    {
        std::string msg = "unknown node with key \"";
        msg += key + "\"";
        msg += " (ingr::instGraph::node ";
        msg += __FILE__;
        msg += " ";
        msg += std::to_string(__LINE__);
        msg += ")";

        throw std::invalid_argument(msg);
    };

    return m_nodes[key];
}

const instGraph::beamMapT & instGraph::beams()
{
    return m_beams;
}

instBeam * instGraph::beam( const std::string & key )
{
    if(m_beams.count(key) != 1)
    {
        std::string msg = "unknown node with key \"";
        msg += key + "\"";
        msg += " (ingr::instGraph::beam ";
        msg += __FILE__;
        msg += " ";
        msg += std::to_string(__LINE__);
        msg += ")";

        throw std::invalid_argument(msg);
    };

    return m_beams[key];
}

int instGraph::constructFromTOMLTable( toml::table & tbl)
{
    toml::array & configNodes = *tbl["nodes"].as_array();

    for(auto&& tab : configNodes)
    {
        toml::node_view<toml::node> name = tab.as_table()->at_path("name");
 
        if(!name.is_string())
        {
            if(tab.source().begin.operator bool())
            {
               std::cerr << "Node at " << tab.source().begin << " has no name. Ignoring. \n";
            }
            else
            {
               std::cerr << "node without name\n";
            }
            continue;
        }
  
        std::string newname = tab.as_table()->at_path("name").as_string()->get();
  
        std::cout << "Creating node: " << newname << "\n";
  
        BREADCRUMB
    
        instNode * newNode = new instNode(newname);

        std::pair<nodeMapT::iterator, bool> nodeRes = m_nodes.emplace(newname, newNode);
  
        BREADCRUMB
  
        toml::array * configOutputs = tab.at_path("outputs").as_array();
        if(configOutputs == nullptr)
        {
           std::cerr << "   Node at " << tab.source().begin << " has no outputs.\n"; 
        }
        else
        {
           std::cout << "\tWith " << configOutputs->size() << " outputs\n";
  
           for( auto&& output : *configOutputs )
           {
              if(! output.as_table()->at_path("name").is_string())
              {
                 /// \todo test me
                 std::cerr << "\tOutput at " << output.source().begin << " has no name.";
                 return -1;
              }
  
              if(! output.as_table()->at_path("beam").is_string())
              {
                 /// \todo test me
                 std::cerr << "\tOutput at " << output.source().begin << " has no beam.";
                 return -1;
              }
  
              BREADCRUMB
  
              //First handle the beam, creating if needed or updating otherwise
              std::string beamName = output.as_table()->at_path("beam").as_string()->get();
  
              ingr::instBeam * newBeam = nullptr;
              
                if(m_beams.count(beamName) == 0)
                {
                    newBeam = new instBeam;
                    std::pair<beamMapT::iterator, bool> beamRes = m_beams.emplace(beamName, newBeam);
                    newBeam->name(beamName);
                    std::cerr << "\tCreated beam " << beamName << "\n";
                }
                else 
                {
                    newBeam = m_beams[beamName];
              
                    if(newBeam != nullptr)
                    {
                        if(newBeam->source() != nullptr)
                        {
                            ///\todo test me
                            std::cerr << "\tBeam " << beamName << "(" << output.as_table()->at_path("beam").node()->source().begin << ") already has a source.\n";
                            return -1;                  
                        
                        }
                    }
  
                    std::cerr << "\tUpdating beam " << beamName << "\n";
                }
  
                BREADCRUMB
  
              //Now handle the output
              std::string putName = output.as_table()->at_path("name").as_string()->get();
  
              BREADCRUMB
  
              //Here we add the output
              instIOPut * newPut = new instIOPut({newNode, ingr::ioDir::output, putName, ingr::putType::light, newBeam});
              newNode->addIOPut(newPut);
           }
        }
  
        toml::array * configInputs = tab.at_path("inputs").as_array();
        if(configInputs == nullptr)
        {
            std::cout << "\tWith " << 0 << " inputs\n";
        }
        else
        {
            std::cout << "\tWith " << configInputs->size() << " inputs\n";
  
            for( auto&& input : *configInputs )
            {
               
                if(! input.as_table()->at_path("name").is_string())
                {
                   /// \todo test me
                   std::cerr << "Input at " << input.source().begin << " has no name.";
                   return -1;
                }
    
                if(! input.as_table()->at_path("beam").is_string())
                {
                   /// \todo test me
                   std::cerr << "Input at " << input.source().begin << " has no beam.";
                   return -1;
                }
    
                BREADCRUMB
    
                //First handle the beam, creating if needed or updating otherwise
                std::string beamName = input.as_table()->at_path("beam").as_string()->get();
    
                instBeam * newBeam = nullptr; 
                
                if(m_beams.count(beamName) == 0)
                {
                    newBeam = new instBeam;
                    std::pair<beamMapT::iterator, bool> beamRes = m_beams.emplace(beamName, newBeam);
                    std::cerr << "\tCreated beam " << beamName << "\n";
                    newBeam->name(beamName);
                }
                else 
                {
                    newBeam = m_beams[beamName];
                    if(newBeam != nullptr)
                    {
                        if(newBeam->dest() != nullptr)
                        {
                            ///\todo test me
                            std::cerr << "Beam " << beamName << "(" << input.as_table()->at_path("beam").node()->source().begin << ") already has a dest.\n";
                            return -1;                  
                        }
                    }
    
                    std::cerr << "\tUpdating beam " << beamName << "\n";
                }
    
                BREADCRUMB
    
                //Now handle the input
                std::string putName = input.as_table()->at_path("name").as_string()->get();
    
                BREADCRUMB
    
                instIOPut * newPut = new instIOPut({newNode, ingr::ioDir::input, putName, ingr::putType::light, newBeam});
                std::string nnKey = newNode->addIOPut(newPut);
    
                if(input.as_table()->at_path("outputLinks").is_array())
                {
                    /// \todo test me
                    std::cerr << "\t\thas outputLinks:\n";
                    for(int n=0; n < input.as_table()->at_path("outputLinks").as_array()->size(); ++n)
                    {
                        std::string olink = input.as_table()->at_path("outputLinks").as_array()->at(n).as_string()->get();
                        std::cerr << "\t\t    " << olink << "\n";

                        newNode->input(nnKey)->outputLink(olink);
                    }
                }
                 
            }
        }
        //End of adding node        
    }

    return 0;
}

int instGraph::constructFromTOMLFile( const std::string & fname )
{
    toml::table tbl;
    try
    {
        tbl = toml::parse_file(fname);
        return constructFromTOMLTable(tbl);
    }
    catch ( const toml::parse_error& err )
    {
        ///\todo identify errors
        std::cerr << "Parsing failed:\n" << err << "\n";
        return -1;
    }
}

} //namespace ingr

