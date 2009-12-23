/** @file camcontrol.h
 *
 * Defines signals which are sent between active objects.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __CAMCONTROL_H__
#define __CAMCONTROL_H__

/** Active object signals */
enum signals {						/**< Description			Parameter		*/

	SIG_KEY_PRESS = Q_USER_SIG,		/**< Key pressed			Key index		*/
	SIG_KEY_RELEASE,				/**< Key release			Key index		*/
	SIG_ENCODER,					/**< Encoder turned			Direction		*/
	SIG_EXECUTE_CMD,				/**< Executes a command		Command			*/
	SIG_SERVO_MOVE,					/**< Moves the servos into	-
									     new position							*/
	SIG_SERVO_DONE,					/**< Servo moved into new	-
									     new position							*/
	SIG_PROG_START,					/**< Starts the program		Program			*/
	SIG_PROG_STOP,					/**< Stops the program		-				*/
	SIG_SHUTTER_START,				/**< Starts the shutter		-				*/
	SIG_SHUTTER_STOP,				/**< Stops the shutter		-				*/
	SIG_SHUTTER_DONE,				/**< Shutter cycle done		-				*/

};

#endif // __CAMCONTROL_H__
