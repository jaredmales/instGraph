#ifndef ingr_instNode_hpp
#define ingr_instNode_hpp

#include <string>
#include <unordered_map>

#include "basicTypes.hpp"
#include "instIOPut.hpp"

namespace ingr
{

/** A node contains inputs and outputs (\ref puts) which connect
  * it to other nodes via beams (\ref instBeam).
  * 
  * \addtogroup nodes
  */

/// A class to represent a node
/** 
  * \ingroup nodes
  */
struct instNode
{

public:
   typedef std::unordered_map<std::string, instIOPut *> ioputMapT;

protected:

   std::string m_name; ///< The unique name of this node
   
   ioputMapT m_inputs; ///< Map of the inputs
   
   ioputMapT m_outputs; ///< Map of the outputs

public:

    /// Default c'tor
    instNode() {}

    /// Copy c'tor
    instNode( const instNode & in /**< [in] the instance to copy*/ );

    /// Destructor
    ~instNode();

    /// Constructor to set the name
    explicit instNode( const std::string & name ///< [in] the unique name of this node
                     ) : m_name{name} {}
 
    /// Get the unique name of this node
    /**
      * \returns the unique name of this node
      */   
    std::string name() const;
 
    /// Set the unique name of this node
    void name(const std::string & nn /**< [in] the new name */);
 
    /// Get the unique key for this node, which is it's name
    /**
      * \returns the unique name of this node
      */ 
    std::string key();
 
    /// Add an input or output to this node
    std::string addIOPut( instIOPut * ip /**< [in] the input or output to add*/);
 
    /// Get a reference to the input map
    /**
      * \returns a reference to the input map (m_inputs)
      */ 
    const ioputMapT & inputs() const;
 
    /// Check if an input pointer is valid
    /**
      * \returns true if m_inputs contains an input identified by key and it is not nullptr
      * \returns false otherwise 
      */
    const bool inputValid( const std::string & key /**< [in] the name of the input*/ ) const;

    /// Get an input by its key
    /**
      * \returns a reference to the input with the given name
      * 
      * \throws std::out_of_range if the pointer is null.  Call inputValid(key) first to check if the pointer is not null.
      */ 
    instIOPut * input( const std::string & key /**< [in] the name of the input*/);
 
    /// Get a reference to the output map
    /**
      * \returns a reference to the output map (m_outputs)
      */
    const ioputMapT & outputs() const;
 
    /// Check if an output pointer is valid
    /**
      * \returns true if m_outputs contains an output identified by key and it is not nullptr
      * \returns false otherwise 
      */
    const bool outputValid( const std::string & key /**< [in] the name of the output*/ ) const;

    /// Get an output by its key
    /**
      * \returns a reference to the output with the given name
      * 
      * \throws std::out_of_range if the pointer is null.  Call outputValid(key) first to check if the pointer is not null.
      */
    instIOPut * output( const std::string & key /**< [in] the name of the output*/);
 
    /// Handle a state change by one of the nodes
    /**
      * Currently a no-op 
     */
    void stateChange();
   
};

} //namespace ingr

#endif  //ingr_instNode_hpp
