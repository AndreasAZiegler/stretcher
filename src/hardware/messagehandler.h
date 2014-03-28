
#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

/**
 * @brief Abstract class for the message handler
 */
class MessageHandler
{
  public:

  /**
   * @brief Start the message handler.
   * @param port Com port
   * @return true if successfull, false otherwise
   */
  bool start (int port);

  /**
   * @brief Receiving method (Should be executed in a own thread.
   */
  void receiver();

private:

  /**
   * @return bool
   * @param  message
   */
  virtual void handler(int message) = 0;

};

#endif // MESSAGEHANDLER_H
