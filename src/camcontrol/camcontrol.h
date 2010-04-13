/** @file camcontrol.h
 *
 * Defines signals which are sent between active objects.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __CAMCONTROL_H__
#define __CAMCONTROL_H__

/** Active object signals */
enum signals {                      /**< Description            Parameter       Source      Destination */

    SIG_KEY_PRESS = Q_USER_SIG,     /**< Key pressed            Key index       key         mmi         */
    SIG_KEY_RELEASE,                /**< Key release            Key index       key         mmi         */
    SIG_ENCODER,                    /**< Encoder turned         Direction       key         mmi         */
    SIG_MMI_CMD,                    /**< Executes a command     Command         mmi         mmi         */
    SIG_MMI_SHOW_MSG,               /**< Shows an info message  Timeout         mmi         mmi         */
    SIG_BACKLIGHT_ACTIVATE,         /**< Activates backlight    -               mmi, key    backlight   */
    SIG_SERVO_MOVE,                 /**< Moves the servos into  -               mmi, prog   servo
                                         new position                                                   */
    SIG_SERVO_DONE,                 /**< Servo moved into new   -               servo       servo, prog
                                         new position                                                   */
    SIG_PROG_START,                 /**< Starts the program     Program         mmi         prog        */
    SIG_PROG_STOP,                  /**< Stops the program      -               mmi         prog        */
    SIG_PROG_DONE,                  /**< Program is done        -               prog        mmi         */
    SIG_PROG_STEP,                  /**< Next step in program   -               prog        prog        */
    SIG_SHUTTER_START,              /**< Starts the shutter     -               prog        shutter     */
    SIG_SHUTTER_STOP,               /**< Stops the shutter      -               prog        shutter     */
    SIG_SHUTTER_DONE,               /**< Shutter cycle done     -               shutter     prog        */
    SIG_SHUTTER_STEP,               /**< Next shot              -               shutter     shutter     */

};

#endif // __CAMCONTROL_H__
