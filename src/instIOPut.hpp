#ifndef ingr_instPut_hpp
#define ingr_instPut_hpp

#include <string>
#include <set>

#include "basicTypes.hpp"

namespace ingr
{

//Forward decls:
struct instBeam;
struct instNode;

/// Class to represent an input or output from a node
/** We refer to inputs and outpus generically as an "IOPut" or a "put". 
  * An IOPut can either be an ioDir::input or ioDir::output (the "direction") and is
  * connected to one \ref instBeam.  The state of a beam's two IOPuts is used
  * to calculate the state of the beam.
  * 
  * A put has a type from \ref putType.
  * 
  * A put has a name which must be unique within it's parent node and it's direction (regardless of type).
  * Note that in principle an input and an output within the same node could have the same name, but
  * an input of putType::light and an input of putType::power must be given unique names.
  * 
  * An input can be linked to one or more outputs, such that a change in the state of the input
  * causes the state of the liniked output to change.
  */
class instIOPut
{

protected:

    instNode * m_node {nullptr}; ///< The node to which this put belongs
    
    ioDir m_io {ioDir::input}; ///< Whether is an input or output. Can have value ioDir::input or ioDir::output;
    
    std::string m_name; ///< The name of this put.
    
    putType m_type {putType::light}; ///< The type of put.  Can have value putLight or putData.
    
    instBeam * m_beam {nullptr}; ///< The beam connected to this put.
    
    putState m_state {putState::off}; ///< The current state of the put: putState::off (default), putState::waiting, or putState::on. 
    
    std::string m_key; ///< The unique key to identify this ioput.
    
    std::set<std::string> m_outputLinks; ///< List of outputs on the same node which are downstream of this input.  This are effectively internal beams. Used for inputs only.
   
public:

    /// Default c'tor
    instIOPut();
    
    /// Full c'tor
    instIOPut( instNode * node,  ///< [in] the parent node
               ioDir io,        ///< [in] the I/O direction
               std::string name, ///< [in] the unique name of this input/output
               putType type,     ///< [in] the type of this input/output
               instBeam * beam   ///< [in] the beam that connects to this input/output
             );
 
    /// Copy c'tor
    instIOPut( const instIOPut & iop /**< [in] the put to copy*/);
  
    /// Setup this IOPut
    /** Sets all values and calculates the key
      * 
      */
    void setup( instNode * node,  ///< [in] the parent node
                ioDir io,        ///< [in] the I/O direction
                std::string name, ///< [in] the unique name of this input/output
                putType type,         ///< [in] the type of this input/output
                instBeam * beam   ///< [in] the beam that connects to this input/output
              );
 
    /// Get the pointer to the parent node
    /**
      * \returns the pointer to the parent node m_node
      */
    instNode * node() const;
 
    /// Set the parent node
    void node( instNode * node /**< [in] address of the parent node */ );
 
    /// Get the direction of this put
    /**
      * \returns the direction of this put m_io
      */
    ioDir io() const;
 
    /// Set the direction of this put
    void io( ioDir iot /**< [in] the new direction*/ );
 
    /// Get the name of this put
    /**
      * \returns the current name m_name
      */
    std::string name() const;
 
    /// Set the name of this put
    void name( const std::string & nm /**< [in] the new name */ );
 
    /// Get the type of this put
    /**
      * \returns the type of this put m_type
      */
    putType type() const;
 
    /// Set the type of this put
    void type( putType t /**< [in] the new type */ );
 
    /// Get the pointer to the beam connected to this put
    /**
      * \returns the beam pointer m_beam
      */
    instBeam * beam() const;
 
    /// Set the pointer to the beam connected to this put
    void beam( instBeam * nb /**< [in] the new beam address */ );
 
    /// Get the current state of this put
    /**
      * \returns the current state m_state
      */
    putState state() const;
 
    /// Change the state of this put
    /** Changes the state, and calls the beam's stateChange method
      * 
      */
    void state( putState ns,        ///< [in] The new state, either putOn, putOff, or putWaiting
                bool nobeam = false /**< [in] [optiona] If true then this only sets the state, 
                                                        does not propagate to the beam.  Outputlinks are updated. */
              );
 
    /// Get the unique key for this put
    /**
      * \returns the current value of m_key
      */
    std::string key() const;
 
    /// Add an output link to this node (if it's an input)
    /**
      * \throws std::logic_error if this is an output node
      */
    void outputLink( const std::string & ol /**< [in] */);
 
    /// Get the set holding hte outputLinks
    /**
      * \returns a const reference to the m_outputLnks set
      */
    const std::set<std::string> & outputLinks();
 
    /// Process a state change from a connected beam.
    /** This is called by the beam.  This calls the parent node's state change function.
      * 
      */
    void stateChange();

protected:

    /// Calculate this IOPuts' unique key from the configured members.
    void makeKey();

};

} //namespace ingr

#endif// ingr_instIOPut_hpp
