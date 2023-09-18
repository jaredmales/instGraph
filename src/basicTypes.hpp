#ifndef ingr_basicTypes_hpp
#define ingr_basicTypes_hpp

#include <string>

namespace ingr
{

/** \name basic_types The Basic Types In An Instrument Graph
  * @{
 */

/// The possible directions of an IOPut
enum class ioDir { input,  ///< an input
                   output  ///< an output
                 };

/// The possible states of an IOPut
enum class putState { off,     ///< The put is off
                      waiting, ///< The put would be on, but is waiting for something upstream
                      on       ///< The put is on
                    };
               
/// The possible types of an IOPut 
enum class putType { light,        ///< an input/output for light
                     data,         ///< an input/output for data
                     power,        ///< an input/output for electrical power
                     mechanical    ///< an input/output for a mechanical connection
                   };

/// The possible states of a beam
enum class beamState { off,          ///< The beam is off
                       intermediate, ///< The beam is intermediate.  This mans the input is on, but the output is off.
                       on            ///< The beam is on
                     };

/** \name state_string String Representations of Types and States
  * @{ 
  */

/// Get a string representation of an IOPut's type
/**
  * \returns a string with value "input" or "output"
  */
inline 
std::string ioDir2String( ioDir type /**< [in] the IOPut type*/)
{
    if(type == ioDir::input) return "input";
    else if(type == ioDir::output) return "output";
    else return "unknown"; 
}

/// Get a character representation of an IOPut's type
/**
  * \returns a character with value i or o.
  */
inline 
char ioDir2Char( ioDir type /**< [in] the IOPut type*/)
{
    if(type == ioDir::input) return 'i';
    else if(type == ioDir::output) return 'o';
    else return '?'; 
}

/// Get a string representation of an IOPut's state
/**
  * \returns a string with value "off", "waiting", or "on". 
  */
inline 
std::string putState2String( putState state /**< [in] the IOPut state*/)
{
    if(state == putState::off) return "off";
    else if(state == putState::waiting) return "waiting";
    else if(state == putState::on) return "on";
    else return "unk"; 
}

/// Get a string representation of an IOPut's type
/**
  * \returns a string with value "light", "data", "power", or "mechanical"
  */
inline 
std::string putType2String( putType type /**< [in] the IOPut type*/)
{
    if(type == putType::light) return "light";
    else if(type == putType::data) return "data";
    else if(type == putType::power) return "power";
    else if(type == putType::mechanical) return "mechanical";
    else return "unknown"; 
}

/// Get a character representation of an IOPut's type
/**
  * \returns a character with value l, d, p, or m.
  */
inline 
char putType2Char( putType type /**< [in] the IOPut type*/)
{
    if(type == putType::light) return 'l';
    else if(type == putType::data) return 'd';
    else if(type == putType::power) return 'p';
    else if(type == putType::mechanical) return 'm';
    else return '?'; 
}

/// Get a string representation of an beam's state
/**
  * \returns a string with value "off", "int", or "on". 
  */
inline
std::string beamState2String( beamState state /**< [in] the beam state*/)
{
    if(state == beamState::off) return "off";
    else if(state == beamState::intermediate) return "int";
    else if (state == beamState::on) return "on";
    else return "unk"; 
}

///@}
///@}

} //namespace ingr

#endif // ingr_basicTypes_hpp