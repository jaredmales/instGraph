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
    std::string m_name;         ///< The unique name of this node

    ioputMapT m_inputs;         ///< Map of the inputs

    ioputMapT m_outputs;        ///< Map of the outputs

    void *m_auxData{ nullptr }; ///< Auxilliary data for this node, i.e. for GUI support.

  public:
    /// Default c'tor
    instNode()
    {
    }

    /// Copy c'tor
    instNode( const instNode &in /**< [in] the instance to copy*/ );

    /// Destructor
    ~instNode();

    /// Constructor to set the name
    explicit instNode( const std::string &name ///< [in] the unique name of this node
                       )
        : m_name{ name }
    {
    }

    /// Get the unique name of this node
    /**
     * \returns the unique name of this node
     */
    std::string name() const;

    /// Set the unique name of this node
    void name( const std::string &nn /**< [in] the new name */ );

    /// Get the unique key for this node, which is it's name
    /**
     * \returns the unique name of this node
     */
    std::string key();

    /// Add an input or output to this node
    std::string addIOPut( instIOPut *ip /**< [in] the input or output to add*/ );

    /// Get a reference to the input map
    /**
     * \returns a reference to the input map (m_inputs)
     */
    const ioputMapT &inputs() const;

    /// Check if an input pointer is valid
    /**
     * \returns true if m_inputs contains an input identified by key and it is not nullptr
     * \returns false otherwise
     */
    bool inputValid( const std::string &key /**< [in] the name of the input*/ ) const;

    /// Get an input by its key
    /**
     * \returns a reference to the input with the given name
     *
     * \throws std::invalid_argument if key is not in m_inputs
     * \throws std::out_of_range if the pointer is null.  Call inputValid(key) first to check if the pointer is not
     * null.
     */
    instIOPut *input( const std::string &key /**< [in] the name of the input*/ );

    /// Get a reference to the output map
    /**
     * \returns a reference to the output map (m_outputs)
     */
    const ioputMapT &outputs() const;

    /// Check if an output pointer is valid
    /**
     * \returns true if m_outputs contains an output identified by key and it is not nullptr
     * \returns false otherwise
     */
    bool outputValid( const std::string &key /**< [in] the name of the output*/ ) const;

    /// Get an output by its key
    /**
     * \returns a reference to the output with the given name
     *
     * \throws std::invalid_argument if key is not in m_outputs
     * \throws std::out_of_range if the pointer is null.  Call outputValid(key) first to check if the pointer is not
     * null.
     */
    instIOPut *output( const std::string &key /**< [in] the name of the output*/ );

    /// Update the outputLinked flag of all outputs
    /** This should be called once after configuration of the node is complete.
     * This is necessary because an output might not exist when an outputLink to
     * it is created in an input.
     *
     */
    void updateOutputLinks();

    /// Check state of all output links that link to a specific output
    /** Checks the state of each input which links to this output.  The output
     * is turned on if any of these inputs is on, otherwise it is waiting if
     * any of these inputs is waiting.  It is only off if all linked inputs are
     * off.
     *
     * This is to deal with the problem of the latest event turning `off` an output node
     * via an output link when it should be `on` due to a different output link.  Called
     * from within instIOPut::state.
     *
     * \todo hould upgrade outputLinks to hold both the input and output to make this faster.
     */
    void checkOutputLinks( const std::string op /**< [in] the output to check outputLinks for */ );

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
    void *auxData();

    /// Set the aux data pointer
    void auxData( void *ad /**< [in] the new aux data pointer */ );

    /// Handle a state change by one of the nodes
    /**
     * Currently a no-op
     */
    void stateChange();
};

} // namespace ingr

#endif // ingr_instNode_hpp
