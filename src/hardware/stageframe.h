
#ifndef STAGEFRAME_H
#define STAGEFRAME_H

#include <vector>
#include "linearstage.h"
#include "linearstagemessagehandler.h"

class StageFrame
{
	public:
    StageFrame();

    void registerLinearStages(std::vector<LinearStage*> *linearstages);

    /**
     * @brief Moves the stage forward frame at constant speed
     */
    void moveForward(double speed = 0);

    /**
     * @brief Moves the stage backward frame at constant speed
     */
    void moveBackward(double speed = 0);

    /**
     * @brief Moves the stage the amount of millimeters.
     * @param milimeters Amount of milimeters
     */
    void moveMM(double millimeters);

    /**
     * @brief Calculate the amount of steps, that the motors have to move to reach the desired distance
     *        and start the motors.
     * @param distance Desired clamping distance in micro steps from the GUI
     */
    void gotoMMDistance(int mmDistance);

    /**
     * @brief Stops the stage frame.
     */
    void stop();

  private:

    /**
     * @todo Implementation
     * @brief Get the current distance.
     */
    double getCurrentDistance(void);

    std::vector<LinearStage*> *m_LinearStages;
    std::vector<LinearStageMessageHandler*> m_LinearStagesMessageHandlers;

    const double MM_PER_MS;               						/**< milimeter per microstep */

    double m_Stepsize;			     				    					/**< Stepsize of the stepper motor in millimeters */
    double m_ZeroDistance;	 	    			           	  /**< Distance when the motors are on max position (resulting in smallest distance) */
};

#endif // STAGEFRAME_H
