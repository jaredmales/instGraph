#ifndef instGraph_hpp
#define instGraph_hpp

#include <map>
#include <string>

#include "instNode.hpp"
#include "instBeam.hpp"

namespace ingr
{

/// A class to hold an instrument graph.
/**
 * \ingroup explainer
 */
class instGraph
{

  public:
    /// A map of nodes
    typedef std::map<std::string, ingr::instNode *> nodeMapT;

    /// A map of beams
    typedef std::map<std::string, ingr::instBeam *> beamMapT;

  protected:
    /// The nodes of this graph
    nodeMapT m_nodes;

    /// The beams of this graph
    beamMapT m_beams;

  public:
    /// Default c'tor
    instGraph();

    /// Destructor
    ~instGraph();

    /// Get a const reference to the node map.
    /**
     * \returns a reference to m_nodes
     */
    const nodeMapT &nodes();

    /// Check if the node pointer indexed by \p key is valid
    /**
     * \returns true if key is in m_modes and the pointer is not null
     * \returns false otherwise
     */
    bool nodeValid( const std::string &key /**< [in] the identifying key for the node */ ) const;

    /// Get the pointer to the node index by \p key
    /** \note check whether the node exists and the pointer is valid using \ref nodeValid(const std::string &) before
     *       calling this function.
     *
     * \returns the pointer stored in m_nodes under \p key
     *
     * \throws std::invalid_argument if key is not in m_nodes
     * \throws std::out_of_range if the pointer is null.
     */
    instNode *node( const std::string &key );

    /// Get a const reference to the beam map.
    /**
     * \returns a reference to m_beams
     */
    const beamMapT &beams();

    /// Check if the beam pointer indexed by \p key is valid
    /**
     * \returns true if key is in m_beam and the pointer is not null
     * \returns false otherwise
     */
    bool beamValid( const std::string &key /**< [in] the identifying key for the beam */ ) const;

    /// Get the pointer to the beam index by \p key
    /** \note check whether the beam exists and the pointer is valid using \ref beamValid(const std::string &) before
     *       calling this function.
     *
     * \returns the pointer stored in m_beams under \p key
     *
     * \throws std::invalid_argument if key is not in m_nodes
     * \throws std::out_of_range if the pointer is null.
     */
    instBeam *beam( const std::string &key );

    virtual void stateChange();

}; // class instGraph

}; // namespace ingr

#endif // instGraph_hpp
