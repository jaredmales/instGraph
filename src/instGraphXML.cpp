#include <iostream>

#include "instGraphXML.hpp"

#define PUGIXML_HEADER_ONLY
#include "pugixml/pugixml.hpp"


namespace ingr
{

#define MXGPARSE_ERR_NODE_NTL (-20)
#define MXGPARSE_ERR_NODE_NNF (-25)
#define MXGPARSE_ERR_NODE_NON (-30)

int parseNode( std::string & name,
               std::string & emsg,
               const std::string & value,
               const size_t & fc
             )
{
    name = "";
    emsg = "";

    if(fc > 4)
    {
        emsg = "mxCell id starts with 'n' but node is not found . (id=\"" + value + "\")";
        return MXGPARSE_ERR_NODE_NTL;
    }
    else if(fc == 4)
    {
        if(value.substr(0,4) != "node")
        {
            emsg = "mxCell id starts with 'n' but node is not found . (id=\"" + value + "\")";
            return MXGPARSE_ERR_NODE_NNF;
        }
    }    
    else if(fc!=1)
    {
        emsg = "mxCell id starts with 'n' but node is not found . (id=\"" + value + "\")";
        return MXGPARSE_ERR_NODE_NON;
    }
    
    name = value.substr(fc+1);

    return 0;
}

#define MXGPARSE_ERR_PUT_INPVALNL (-55)
#define MXGPARSE_ERR_PUT_INPNOF (-60)
#define MXGPARSE_ERR_PUT_IVALNL (-65)
#define MXGPARSE_ERR_PUT_IPE (-70)
#define MXGPARSE_ERR_PUT_OUTVALNL (-75)
#define MXGPARSE_ERR_PUT_OUTNOF (-80)
#define MXGPARSE_ERR_PUT_OVALNL (-85)
#define MXGPARSE_ERR_PUT_OPE (-90)
#define MXGPARSE_ERR_PUT_LIGHTNF (-95)
#define MXGPARSE_ERR_PUT_LNF (-100)
#define MXGPARSE_ERR_PUT_DATANF (-105)
#define MXGPARSE_ERR_PUT_DNF (-110)
#define MXGPARSE_ERR_PUT_POWERNF (-115)
#define MXGPARSE_ERR_PUT_PNF (-120)
#define MXGPARSE_ERR_PUT_MECHNF (-125)
#define MXGPARSE_ERR_PUT_MNF (-130)
#define MXGPARSE_ERR_PUT_FLUIDNF (-135)
#define MXGPARSE_ERR_PUT_FNF (-140)
#define MXGPARSE_ERR_PUT_TINV (-145)
#define MXGPARSE_ERR_PUT_NO2C (-150)
#define MXGPARSE_ERR_PUT_NONN (-155)
#define MXGPARSE_ERR_PUT_NPN (-160)

int parsePut( ioDir & dir,
              putType & type,
              std::string & node,
              std::string & name,
              std::string & emsg,
              const std::string & value,
              const size_t & fc
            )
{
    emsg = "";

    //First check for put type
    size_t tfc = fc;
    size_t tc = std::string::npos; //type marker
    if(fc > 1)
    {
        tc = value.rfind(fc, '.');
        if(tc != std::string::npos)
        {
            tfc = tc;
        }
    }
    // here if tc != npos then there is a type

    //Get direction (input or output)
    if(value[0] == 'i')
    {
        if(tfc == 5)
        {
            if(value.length() < 9) //validate that there is at least 'input:n:n'
            {
                emsg = "mxCell input id value isn't long enough, must be at least input:n:n (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_INPVALNL; 
            }

            if(value.substr(0,5) != "input")
            {
                emsg = "mxCell input id value parse error (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_INPNOF;
            }
        }
        else if(tfc == 1)
        {
            if(value.length() < 5) //validate that there is at least 'i:n:n'
            {
                emsg = "mxCell input id value isn't long enough, must be at east i:n:n (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_IVALNL; 
            }
        }
        else
        {
            emsg = "mxCell input id value parse error (id=\"" + value  + "\")";
            return MXGPARSE_ERR_PUT_IPE; //it's neither "input" nor "i"
        }

        dir = ioDir::input;
    }
    else if(value[0] == 'o')
    {
        if(tfc == 6)
        {
            if(value.length() < 10) //validate that there is at least 'output:n:n'
            {
                emsg = "mxCell output id value isn't long enough, must be at least output:n:n (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_OUTVALNL; 
            }

            if(value.substr(0,6) != "output")
            {
                emsg = "mxCell output id value parse error (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_OUTNOF;
            }
        }
        else if(tfc == 1)
        {
            if(value.length() < 5) //validate that there is at least 'i:n:n'
            {
                emsg = "mxCell input id value isn't long enough, must be at least o:n:n (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_OVALNL; 
            }
        }
        else
        {
            emsg = "mxCell output id value parse error (id=\"" + value  + "\")";
            return MXGPARSE_ERR_PUT_OPE; //it's neither "output" nor "o"
        }

        dir = ioDir::output;
    }

    if(tc != std::string::npos)
    {
        //Now check for l, d, p, m, f

        tc += 1;
        if(value[tc] == 'l')
        {
            if(fc - tc == 5)
            {
                if(value.substr(tc,5) != "light")
                {
                    emsg = "mxCell id 'type' parse error, light not found (id=\"" + value  + "\")";
                    return MXGPARSE_ERR_PUT_LIGHTNF; 
                }
            }
            else if(fc-tc != 1)
            {
                emsg = "mxCell id 'type' parse error, light not found (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_LNF; 
            }

            type = putType::light;
        }
        else if(value[tc] == 'd')
        {
            if(fc - tc == 4)
            {
                if(value.substr(tc,4) != "data")
                {
                    emsg = "mxCell id 'type' parse error, data not found (id=\"" + value  + "\")";
                    return MXGPARSE_ERR_PUT_DATANF; 
                }
            }
            else if(fc-tc != 1)
            {
                emsg = "mxCell id 'type' parse error, data not found (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_DNF; 
            }

            type = putType::data;
        }
        else if(value[tc] == 'p')
        {
            if(fc - tc == 5)
            {
                if(value.substr(tc,4) != "power")
                {
                    emsg = "mxCell id 'type' parse error, power not found (id=\"" + value  + "\")";
                    return MXGPARSE_ERR_PUT_POWERNF; 
                }
            }
            else if(fc-tc != 1)
            {
                emsg = "mxCell id 'type' parse error, power not found (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_PNF; 
            }

            type = putType::power;
        }
        else if(value[tc] == 'm')
        {
            if(fc - tc == 10)
            {
                if(value.substr(tc,4) != "mechanical")
                {
                    emsg = "mxCell id 'type' parse error, mechanical not found (id=\"" + value  + "\")";
                    return MXGPARSE_ERR_PUT_MECHNF; 
                }
            }
            else if(fc-tc != 1)
            {
                emsg = "mxCell id 'type' parse error, mechanical not found (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_MNF; 
            }

            type = putType::mechanical;
        }
        else if(value[tc] == 'f')
        {
            if(fc - tc == 5)
            {
                if(value.substr(tc,4) != "fluid")
                {
                    emsg = "mxCell id 'type' parse error, fluid not found (id=\"" + value  + "\")";
                    return MXGPARSE_ERR_PUT_FLUIDNF; 
                }
            }
            else if(fc-tc != 1)
            {
                emsg = "mxCell id 'type' parse error, fluid not found (id=\"" + value  + "\")";
                return MXGPARSE_ERR_PUT_FNF; 
            }

            type = putType::fluid;
        }
        else 
        {
            emsg = "mxCell id value 'type' is not valid (id=\"" + value  + "\")";
            return MXGPARSE_ERR_PUT_TINV; 
        }
    }
    else 
    {
        type = putType::light;
    }

    size_t sc = value.find(':', fc+1);
    if(sc == std::string::npos)
    {
        emsg = "mxCell ioput without second ':' (id=\"" + value  + "\")";
        return MXGPARSE_ERR_PUT_NO2C;
    }

    if(sc == fc+1)
    {
        emsg = "mxCell ioput without node name (id=\"" + value  + "\")";
        return MXGPARSE_ERR_PUT_NONN;
    }

    if(sc == value.length()-1)
    {
        emsg = "mxCell ioput without ioput name (id=\"" + value  + "\")";
        return MXGPARSE_ERR_PUT_NPN;
    }   

    node = value.substr(fc+1, sc-(fc+1));
    name = value.substr(sc+1);

    return 0;
}

#define MXGPARSE_ERR_BEAM_NOBEAM (-200)
#define MXGPARSE_ERR_BEAM_NOB (-205)
#define MXGPARSE_ERR_BEAM_NON (-210)
#define MXGPARSE_ERR_BEAM_NOS (-215)
#define MXGPARSE_ERR_BEAM_NOO (-220)
#define MXGPARSE_ERR_BEAM_NOOC (-225)
#define MXGPARSE_ERR_BEAM_OPE (-2000)
#define MXGPARSE_ERR_BEAM_ODIR (-230)

#define MXGPARSE_ERR_BEAM_NOT (-235)
#define MXGPARSE_ERR_BEAM_NOI (-240)
#define MXGPARSE_ERR_BEAM_NOIC (-245)
#define MXGPARSE_ERR_BEAM_IPE (-3000)
#define MXGPARSE_ERR_BEAM_IDIR (-250)

#define MXGPARSE_ERR_BEAM_TYPES (-255)

int parseBeam( std::string & name,
               std::string & outNode,
               std::string & outName,
               std::string & inNode,
               std::string & inName,
               std::string & emsg,
               const std::string & value,
               const size_t & fc,
               pugi::xml_attribute & source,
               pugi::xml_attribute & target,
               const std::string & beamStr = "beam"
             )
{
    if(fc == beamStr.size())
    {
        if(value.substr(0,beamStr.size()) != beamStr)
        {
            emsg = "mxCell id " + beamStr + " not found (id=\"" + value  + "\")";
            return MXGPARSE_ERR_BEAM_NOBEAM;
        }
    }
    else if(fc != 1)
    {
        emsg = "mxCell id " + beamStr + " not found (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NOB;
    }

    name = value.substr(fc+1);

    if(name.size() == 0)
    {
        emsg = "mxCell id " + beamStr + " name not found (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NON;
    }
    
    if(source.empty())
    {
        emsg = "mxCell id " + beamStr + " sourcenot found (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NOS;
    }

    ioDir odir;
    putType otype;
    std::string pemsg;
    std::string pvalue = source.value();

    if(pvalue[0] != 'o')
    {
        emsg = "mxCell id " + beamStr + " output not found in source (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NOO;
    }
    
    size_t pfc = pvalue.find(":");

    if(pfc == std::string::npos)
    {
        emsg = "mxCell id " + beamStr + " output not found in source (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NOOC;
    }
    
    int pec = parsePut(odir, otype, outNode, outName, pemsg, pvalue, pfc);

    if(pec < 0)
    {
        emsg = beamStr + " output parse error: " + pemsg;
        return MXGPARSE_ERR_BEAM_OPE + pec;
    }

    if(odir != ioDir::output)
    {
        emsg = beamStr + " source is not an output (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_ODIR;
    }

    if(target.empty())
    {
        emsg = "mxCell id " + beamStr + " target not found (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NOT;
    }

    ioDir idir;
    putType itype;
    pvalue = target.value();

    if(pvalue[0] != 'i')
    {
        emsg = "mxCell id " + beamStr + " input not found in target (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NOI;
    }
    
    pfc = pvalue.find(":");

    if(pfc == std::string::npos)
    {
        emsg = "mxCell id " + beamStr + " input not found in target (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_NOIC;
    }
    
    pec = parsePut(idir, itype, inNode, inName, pemsg, pvalue, pfc);

    if(pec < 0)
    {
        emsg = beamStr + " input parse error: " + pemsg;
        return MXGPARSE_ERR_BEAM_IPE + pec;
    }

    if(idir != ioDir::input)
    {
        emsg = beamStr + " target is not an input (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_IDIR;
    }

    if(otype != itype) 
    {
        emsg = beamStr + " source and target type mismatch (id=\"" + value  + "\")";
        return MXGPARSE_ERR_BEAM_TYPES;
    }

    return 0;

}



//walker struct for depth traversal to look for the mxGraphModel
struct find_mxGraph: pugi::xml_tree_walker
{
    pugi::xml_node mxGraph;
    virtual bool for_each(pugi::xml_node& node)
    {
        if(std::string(node.name()) == "mxGraphModel")
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
    if(m_doc)
    {
        delete m_doc;
    }
}

#define MXGPARSE_ERR_DOC_NOMXG (-500)
#define MXGPARSE_ERR_DOC_NOROOT (-505)
#define MXGPARSE_ERR_DOC_CE (-510)
#define MXGPARSE_ERR_DOC_SE (-515)
#define MXGPARSE_ERR_DOC_OLDN (-520)

int instGraphXML::parseXMLDoc( std::string & emsg )
{
    find_mxGraph mxGraph;
    m_doc->traverse(mxGraph);

    if(mxGraph.mxGraph.empty())
    {
        emsg = "no mxGraphModel found in doc";
        return MXGPARSE_ERR_DOC_NOMXG;
    }

    pugi::xml_node root = mxGraph.mxGraph.child("root");

    if(root.empty())
    {
        emsg = "no root found in mxGraphModel";
        return MXGPARSE_ERR_DOC_NOROOT;
    }

    for (pugi::xml_node cell: root.children("mxCell"))
    {
        pugi::xml_attribute id = cell.attribute("id");
        if(id.name()[0] == '\0') 
        {
            continue;
        }
        
        std::string value = id.as_string("");
        if(value.length() < 1)
        {
            emsg = "mxCell with empty id (length 0)";
            return MXGPARSE_ERR_DOC_CE;
        }

        size_t fc = value.find(':');
        if(fc == std::string::npos) // not an instGraph cell
        {
            //This could just be id="0" or id="1", or some other entity of the graph
            continue;
        }
    
        if(fc == 0 || fc == value.length()) //starts or ends with :
        {
            emsg = "mxCell id starts or ends with ':'. (id=\"" + value + "\")";
            return MXGPARSE_ERR_DOC_SE;
        }

        if(value[0] == 'n')
        {
            std::string name;

            int ec = parseNode(name, emsg, value, fc);
            if( ec < 0)
            {
                return ec;
            }

            instNode * newNode = new instNode(name);

            std::pair<nodeMapT::iterator, bool> nodeRes = m_nodes.emplace(name, newNode);
            
        }
        else if(value[0] == 'o' || value[0] == 'i')
        {
            ioDir dir;
            putType type;
            std::string node;
            std::string name;

            int ec = parsePut(dir, type, node, name, emsg, value, fc);
            if( ec < 0)
            {
                return ec;
            }

            if(m_nodes.count(node) == 0) //node might not exist b/c it hasn't been parsed yet
            {
                instNode * newNode = new instNode(node);

                std::pair<nodeMapT::iterator, bool> nodeRes = m_nodes.emplace(node, newNode);
            }
            
            instNode * newNode = m_nodes[node];

            instIOPut * newPut = new instIOPut({newNode, dir, name, type, nullptr});
            newNode->addIOPut(newPut);
        }
        else if(value[0] == 'b')
        {
            std::string name;
            std::string outNode;
            std::string outName;
            std::string inNode;
            std::string inName;
            
            pugi::xml_attribute source = cell.attribute("source");
            pugi::xml_attribute target = cell.attribute("target");

            int ec = parseBeam(name, outNode, outName, inNode, inName, emsg, value, fc, source, target);

            if(ec < 0)
            {
                return ec;
            }
            
            instBeam * newBeam = new instBeam;
            std::pair<beamMapT::iterator, bool> beamRes = m_beams.emplace(name, newBeam);
            newBeam->name(name);

            m_nodes[outNode]->output(outName)->beam(newBeam);

            newBeam->source(m_nodes[outNode]->output(outName));

            m_nodes[inNode]->input(inName)->beam(newBeam);
            newBeam->dest(m_nodes[inNode]->input(inName));
        }
        else if(value[0] == 'l')
        {
            std::string name;
            std::string outNode;
            std::string outName;
            std::string inNode;
            std::string inName;
            
            pugi::xml_attribute source = cell.attribute("source");
            pugi::xml_attribute target = cell.attribute("target");

            //note that when calling for a link instead of a beam we swap target and source
            int ec = parseBeam(name, outNode, outName, inNode, inName, emsg, value, fc, target, source, "link");

            if(ec < 0)
            {
                return ec;
            }
            
            if(outNode != inNode)
            {
                 emsg = "output link has different nodes for source and target ':'. (id=\"" + value + "\")";
                 return MXGPARSE_ERR_DOC_OLDN;
            }
            m_nodes[inNode]->input(inName)->outputLink(outName);
        }
    }

    return 0;
}

int instGraphXML::parseXMLDoc( std::string & emsg,
                               const pugi::xml_document & doc 
                             )
{
    m_doc->reset(doc);

    return parseXMLDoc(emsg);
}

int instGraphXML::loadXMLFile( std::string & emsg,
                               const std::string & fname 
                             )


{
    if (!m_doc->load_file(fname.c_str())) 
    {
        emsg = "error loading file " + fname;
        return -1;
    }

    return parseXMLDoc( emsg );
}

} //namespace ingr
