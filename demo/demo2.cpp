

#include <iostream>

#include "../src/instGraphXML.hpp"


int main()
{
    ingr::instGraphXML igr;
    igr.outputPath("tmp.drawio");
    
    std::cerr << "######################################\n";
    std::cerr << " building graph\n";
    std::cerr << "######################################\n\n";

    std::string emsg;
    int ec = igr.loadXMLFile(emsg, "demo2.drawio");
    
    if(ec < 0)
    {
        std::cerr << "demo2: error\n  " << emsg << "\n  code: " << ec << std::endl;
        return ec;
    }

    std::cerr << "######################################\n";
    std::cerr << " confirming graph\n";
    std::cerr << "######################################\n\n";

    std::cerr << "nodes.size() = " << igr.nodes().size() << "\n";
    std::cerr << "beams.size() = " << igr.beams().size() << "\n";

    for(auto it=igr.nodes().begin(); it!=igr.nodes().end();++it)
    {
        std::cerr << "  node: " << it->second->name() << "\n";
        std::cerr << "    inputs: " << it->second->inputs().size() << "\n";
        for(auto&& iit : it->second->inputs())
        {
            std::cerr << "      " << iit.second->name() << " (" << ioDir2String(iit.second->io()) << ") <-- ";
            if(iit.second->beamValid())
            {
                std::cerr << iit.second->beam()->name();
            }
            std::cerr << "\n";
        }
        std::cerr << "    outputs: " << it->second->outputs().size() << "\n";
        for(auto&& oit : it->second->outputs())
        {
            std::cerr << "      " << oit.second->name() << " (" << ioDir2String(oit.second->io()) << ") --> ";
            if(oit.second->beamValid()) 
            {
                std::cerr << oit.second->beam()->name();
            }
            std::cerr << "\n";
        }
        std::cerr << "\n";
    }
 
    char c;

    std::cerr << "######################################\n";
    std::cerr << " running demo\n";
    std::cerr << "######################################\n\n";

    std::cout << "Step 0: all off:\n  ";
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";
    igr.stateChange();

    std::cout << "press enter . . .";
    c = getchar();
    std::cout << "\n\n";

    std::cout << "Step 1: source.out on:\n  ";
    igr.node("source")->output("out")->state(ingr::putState::on);
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";
 
    std::cout << "press enter . . .";
    c = getchar();
    std::cout << "\n\n";

    std::cout << "Step 2: fwtelsim.in on:\n  ";
    igr.node("fwtelsim")->input("in")->state(ingr::putState::on);
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";
 
    std::cout << "press enter . . .";
    c = getchar();
    std::cout << "\n\n";

    std::cout << "Step 3: stagek.in on:\n  ";
    igr.node("stagek")->input("in")->state(ingr::putState::on);
    std::cout << "stagek.in state  = " << ingr::putState2String(igr.node("stagek")->input("in")->state()) << "\n  ";
    std::cout << "stagek.out state = " << ingr::putState2String(igr.node("stagek")->output("out")->state()) << "\n  ";
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";

    std::cout << "press enter . . .";
    c = getchar();
    std::cout << "\n\n";

    std::cout << "Step 4: pickoff.lab on:\n  ";
    igr.node("pickoff")->input("lab")->state(ingr::putState::on);
    std::cout << "stagek.in state  = " << ingr::putState2String(igr.node("stagek")->input("in")->state()) << "\n  ";
    std::cout << "stagek.out state = " << ingr::putState2String(igr.node("stagek")->output("out")->state()) << "\n  ";
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";
 
    std::cout << "press enter . . .";
    c = getchar();
    std::cout << "\n\n";

    std::cout << "Step 5: source.out off:\n  ";
    igr.node("source")->output("out")->state(ingr::putState::off);
    std::cout << "pickoff.lab state  = " << ingr::putState2String(igr.node("pickoff")->input("lab")->state()) << "\n  ";
    std::cout << "stagek.in state  = " << ingr::putState2String(igr.node("stagek")->input("in")->state()) << "\n  ";
    std::cout << "stagek.out state = " << ingr::putState2String(igr.node("stagek")->output("out")->state()) << "\n  ";
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";
 
    std::cout << "press enter . . .";
    c = getchar();
    std::cout << "\n\n";

    std::cout << "Step 6: tel.out on:\n  ";
    igr.node("telescope")->output("out")->state(ingr::putState::on);
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";
    
    std::cout << "press enter . . .";
    c = getchar();
    std::cout << "\n\n";

    std::cout << "Step 7: pickoff.lab off, pickoff.tel on:\n  ";
    igr.node("pickoff")->input("lab")->state(ingr::putState::off);
    igr.node("pickoff")->input("tel")->state(ingr::putState::on);
    std::cout << "source2fwtelsim state   = " << ingr::beamState2String(igr.beam("source2fwtelsim")->state()) << "\n  ";
    std::cout << "fwtelsim2pickoff state  = " << ingr::beamState2String(igr.beam("fwtelsim2pickoff")->state()) << "\n  ";
    std::cout << "telescope2pickoff state = " << ingr::beamState2String(igr.beam("telescope2pickoff")->state()) << "\n  ";
    std::cout << "pickoff2stagek state    = " << ingr::beamState2String(igr.beam("pickoff2stagek")->state()) << "\n";

    return 0;
}