#ifndef ingr_instBeam_hpp
#define ingr_instBeam_hpp

#include <string>

#include "instIOPut.hpp"

namespace ingr
{

struct instNode;

/// Class to represent a propagation path
/** An `instBeam` connects an output of one node to an input of a different node.  It can have a state of off, 
  * intermediate, or on, which depends on the states of the input and output.
  * 
  * \ingroup beams
  */ 
class instBeam
{

protected:
    std::string m_name; ///< The name of this beam, must be unique.

    instIOPut *m_source {nullptr}; ///< The input
   
    instIOPut *m_dest {nullptr}; ///< The output

    beamState m_state; ///< The current state of the beam, calculated on a call to stateChange() from the states of the inputs.  Will have one of the values of beamState::off (default), beamState::intermediate, or beamState::on.

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
 
    /// Get the pointer to the input to the beam
    /**
      * \returns the pointer to the input to the beam 
      */
    instIOPut * source();
 
    /// Set the pointer to the input
    void source(instIOPut * inp /**<[in] the new input pointer */ );
 
    /// Get the pointer to the output of the beam
    /**
      * \returns the pointer to the output of the beam 
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
