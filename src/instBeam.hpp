#ifndef ingr_instBeam_hpp
#define ingr_instBeam_hpp

#include <string>

#include "instIOPut.hpp"

namespace ingr
{

//Forward
class instGraph;

/// Class to represent a propagation path
/** An `instBeam` connects an output of one node to an input of a different node.
  * It can have a state of off, intermediate, or on, which depends on the 
  * states of the input and output.
  * 
  * \ingroup beams
  */ 
class instBeam
{

protected:
    std::string m_name; ///< The name of this beam, must be unique.

    instIOPut *m_source {nullptr}; ///< The input
   
    instIOPut *m_dest {nullptr}; ///< The output

    beamState m_state; /**< The current state of the beam, calculated on 
                            a call to stateChange() from the states of the 
                            inputs.  Will have one of the values of 
                            beamState::off (default), beamState::intermediate, 
                            or beamState::on. */

    instGraph * m_parentGraph {nullptr}; ///< Pointer to the parent instGraph that holds this beam

    void * m_auxData {nullptr}; ///< Auxilliary data for this beam, i.e. for GUI support.

public:

    /// Default c'tor
    instBeam();

    /// Copy c'tor
    instBeam( const instBeam & ib /**< [in] the instance to copy */);

    /// Get the name of the beam
    /**
      * \returns the name of the beam 
      */ 
    std::string name();

    /// Set the name of the beam
    void name(const std::string & n /**< [in] the new name */ );
 
    /// Check if source pointer is set 
    /**
      * \returns true if the pointer to the source is not null
      * \returns false otherwise 
      */
    const bool sourceValid() const;

    /// Get the pointer to the input to the beam
    /**
      * \returns the pointer to the input to the beam 
      * 
      * \throws std::out_of_range if the pointer is null.  Call sourceValid() to check first.
      */
    instIOPut * source();
 
    /// Set the pointer to the input
    void source(instIOPut * inp /**<[in] the new input pointer */ );
 
    /// Check if dest pointer is set 
    /**
      * \returns true if the pointer to the dest is not null
      * \returns false otherwise 
      */
    const bool destValid() const;
    
    /// Get the pointer to the output of the beam
    /**
      * \returns the pointer to the output of the beam 
      * 
      * \throws std::out_of_range if the pointer is null.  Call destValid() to check first.
      */
    instIOPut * dest();
 
    /// Set the pointer to the output
    void dest(instIOPut * outp /**<[in] the new output pointer */ );
 
    /// Get the state of the beam
    /** Will have one of the values of putStates: putOff, putWaiting, putOn.
      * 
      * \returns the state of the beam 
      */
    beamState state();
 
    /// Set the parent instGraph
    void parentGraph(instGraph * ig /**< [in] pointer to the parent instGraph */);

    /// Check if an aux data pointer is valid
    /**
      * \returns true if m_auxData is not nullptr
      * \returns false otherwise 
      */
    bool auxDataValid();

    /// Get the aux data pointer
    /**
      * \returns the aux data pointer m_auxData
      * 
      * \throws std::out_of_range if the pointer is null.  Call auxDataValid first to check if the pointer is not null.
      */
    void * auxData();

    /// Set the aux data pointer
    void auxData( void * ad /**< [in] the new aux data pointer */);
    
    /// Get the name of the beam
    /**
      * \returns the name of the beam 
      */
    std::string key();
 
    /// Change the state of the beam.
    /** Re-calculates the beam state based on the states of the input and output.
      */
    void stateChange();

};

}//namespace ingr

#endif //ingr_instBeam_hpp
