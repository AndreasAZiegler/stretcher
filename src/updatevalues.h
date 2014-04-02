#ifndef UPDATEVALUES_H
#define UPDATEVALUES_H

class UpdateValues
{
  public:
    enum class ValueType{
      Pos1 = 0,
      Pos2 = 1,
      Force = 2
    };

    UpdateValues();
    UpdateValues(UpdateValues& other){
    }

    virtual void updateValue(int value, UpdateValues::ValueType type) = 0;
};

#endif // UPDATEVALUES_H
