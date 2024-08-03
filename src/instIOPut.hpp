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
class instGraph;

/** We refer to inputs and outputs generically as an "IOPut" or a "put". 
  * An IOPut can either be an ioDir::input or ioDir::output (the "direction") and is
  * connected to one \ref instBeam.  The state of a beam's two IOPuts is used
  * to calculate the state of the beam.
  * 
  * A put has a type from \ref putType.
  * 
  * A put has a name which must be unique within it's parent node and it's direction 
  * (regardless of type). Note that in principle an input and an output within the 
  * same node could have the same name, but an input of putType::light and an input 
  * of putType::power must be given unique names.
  * 
  * An input can be linked to one or more outputs, such that a change in the state 
  * of the input causes the state of the linked output to change.  These are called
  * "output links".
  * 
  * \addtogroup puts
  */  

 
/// Class to represent an input or output from a node
/** 
  * 
  * \ingroup puts
  */
class instIOPut
{

protected:

    instNode * m_node {nullptr}; ///< The node to which this put belongs
    
    ioDir m_io {ioDir::input}; ///< Whether is an input or output. Can have value ioDir::input or ioDir::output;
    
    std::string m_name; ///< The name of this put.
    
    putType m_type {putType::light}; ///< The type of put.  Can have value putLight or putData.
    
    instBeam * m_beam {nullptr}; ///< The beam connected to this put.
    
    bool m_outputLinked {false}; ///< For an output, whether or not an input has an output link to this.

    /** The current state of the put: putState::off (default), putState::waiting, or 
      * putState::on.
      */ 
    putState m_state {putState::off}; 
    
    std::string m_key; ///< The unique key to identify this ioput.
    
    /** List of outputs on the same node which are downstream of this input.  
      * This are effectively internal beams. Used for inputs only.
      */
    std::set<std::string> m_outputLinks; 

    instGraph * m_parentGraph {nullptr}; ///< Pointer to the parent instGraph that holds this beam

    void * m_auxData {nullptr}; ///< Auxilliary data for this beam, i.e. for GUI support.

public:

    /// Default c'tor
    instIOPut();
    
    /// Full c'tor
    instIOPut( instNode * node,  ///< [in] the parent node
               ioDir io,         ///< [in] the I/O direction
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
 
    /// Check if node pointer is set 
    /**
      * \returns true if the pointer to parent node m_node is not null
      * \returns false otherwise 
      */
    bool nodeValid() const;

    /// Get the pointer to the parent node
    /**
      * \returns the pointer to the parent node m_node
      * 
      * \throws std::out_of_range if the pointer is null.  Call nodeValid() to check first.
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
 
    /// Check if beam pointer is set 
    /**
      * \returns true if the pointer to the beam is not null
      * \returns false otherwise 
      */
    bool beamValid() const;

    /// Get the pointer to the beam connected to this put
    /**
      * \returns the beam pointer m_beam
      * 
      * \throws std::out_of_range if the pointer is null.  Call beamValid() to check first.
      */
    instBeam * beam() const;
 
    /// Set the pointer to the beam connected to this put
    void beam( instBeam * nb /**< [in] the new beam address */ );
 
    /// Set the outputLinked flag
    void outputLinked( const bool & ol /**<[in] the new value of the output linked flag */);

    /// Get the value of the outputLinked flag
    /** 
      * \returns the current value of m_outputLinked 
      */
    bool outputLinked() const;

    /// Get the current state of this put
    /**
      * \returns the current state m_state
      */
    putState state() const;
 
    /// Change the state of this put
    /** Changes the state, and calls the beam's stateChange method
      * 
      */
    void state( putState ns,              ///< [in] The new state, either putOn, putOff, or putWaiting
                bool nobeam = false,      /**< [in] [optiona] If true then this only sets the state, does not propagate 
                                                              to the beam.  Outputlinks are updated. */
                bool byOutputlink = false /**< [in] [optional] If false, and this is an output, and m_outputLinked = true,
                                                               then this state call is ignored.  This should only be set 
                                                               to true by a call triggered by an outputLink. */
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
