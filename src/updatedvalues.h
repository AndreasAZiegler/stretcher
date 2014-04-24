#ifndef UPDATEDVALUES_H
#define UPDATEDVALUES_H

// Includes
#include <chrono>

class UpdatedValues
{
  public:

    /**
     * @brief Struct for the measurement values containing the values and the time stamp.
     */
    struct MeasurementValue{
      long value;
      std::chrono::high_resolution_clock::time_point timestamp;
    };

    /**
     * @brief Defines the 3 types of measurement values.
     */
    enum class ValueType{
      Pos1 = 0,
      Pos2 = 1,
      Distance = 2,
      Force = 3
    };

    UpdatedValues();
};

#endif // UPDATEDVALUES_H
