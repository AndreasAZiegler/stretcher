/*
 Copyright (c) 2014, Andreas Ziegler

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.
 This software is provided 'as-is', without any express or implied warranty.
*/

#ifndef _LINEARMOTOR_H__
#define _LINEARMOTOR_H__

// included dependencies
#include "serialinterface.h"

  /** Class that provides access to the hardware module with the zaber linear stepper motor.
  *
  */

  class LinearMotor : public SerialInterface
  {

  public:
    //! Port defaults to com1 and baud rate to 115200
    LinearMotor();
    ~LinearMotor();


   private:
    //! Command strings that persuade the motors to move
    //Some commands need a suffix with data
    // additional commands can be found in the Zaber manual

    // Defined commands for the Zaber linear motors.
    const char *MOTORS_DEVICE_MODE;
    const char *MOTORS_RESET;
    const char *MOTORS_RETURN_CURRENT_POSITION;
    const char *MOTORS_GO_HOME;			//set to home distance (0)
    const char *MOTORS_MOVE_ABSOLUTE;
    const char *MOTORS_MOVE_RELATIVE;
    const char *MOTORS_SET_SPEED;
    const char *MOTORS_MOVE_AT_CONSTANT_SPEED;
    const char *MOTORS_STOP;

    const double MM_PER_MS;               /**< milimeter per microstep */

    const float DecIncSpeed;              /**< Amount of speed to increase/decrease */

    double mStepsize;			           			//!< Stepsize of the stepper motor in millimeters
    char mbytedata[4];
    int mOscstate;							          //!< Current state of the oscillation
    double mAmplitude;						        //!< Amplitude of the oscillation
    bool mStoprequest;						        //!< stopflag for the oscillation
    int mLMtimerID;							          //!< Timer ID of the oscillation timer
    int mCurrentSpeed;                    /**< The current speed */
    int mCurrentPosMotor[2];              /**< Position of the two motors */
    double mZeroDistance;                 /**< Distance when the motors are on max position (resulting in smallest distance) */
    bool mOldPositionFlag;                /**< Flag to indicate that the current "old" position is still up to date */
    bool mMoveFinishedFlag;               /**< Flag to indicate, that the motors finished moving */
    bool mPositionPendingFlag;            /**< Indicate that a RETURN_CURRENT_POSITION command is executed but the answer not yet arrived */
    bool mStartUpFlag;                    /**< Indicates that software is starting up or started up */

    bool mExpectedMessagesFlag;           /**< Indicates that messages are expected */

    //MessagesHandler *mMessagesHandler;    /**< Pointer to the MessagesHandler object */
  };

#endif /* _LINEARMOTOR_H__ */
