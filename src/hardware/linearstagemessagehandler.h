
#ifndef LINEARSTAGEMESSAGEHANDLER_H
#define LINEARSTAGEMESSAGEHANDLER_H

// Includes
#include "messagehandler.h"

/**
 * @brief Message handler for the linear stages
 */
class LinearStageMessageHandler : virtual public MessageHandler
{
  public:

    /**
     * @brief Informs the related objects according to the received message
     * @param message Received message
     */
    void handler(int message);

};

#endif // LINEARSTAGEMESSAGEHANDLER_H
