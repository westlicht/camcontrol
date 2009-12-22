#ifndef __CAMCONTROL_H__
#define __CAMCONTROL_H__

/** Active object signals */
enum signals {						/**< Description			Parameter		*/

	SIG_KEY_PRESS = Q_USER_SIG,		/**< Key pressed			Key index		*/
	SIG_KEY_RELEASE,				/**< Key release			Key index		*/
	SIG_ENCODER,					/**< Encoder turned			Direction		*/
	SIG_EXECUTE_CMD,				/**< Executes a command		Command			*/
	SIG_PROG_START,					/**< Starts the program		Program			*/
	SIG_PROG_STOP,					/**< Stops the program		-				*/
	SIG_SHUTTER_START,				/**< Starts the shutter		-				*/
	SIG_SHUTTER_STOP,				/**< Stops the shutter		-				*/
	SIG_SHUTTER_DONE,				/**< Shutter cycle done		-				*/

};

/* Active objects */
extern struct mmi_ao mmi_ao;
extern struct prog_ao prog_ao;
extern struct shutter_ao shutter_ao;
extern struct servo_ao servo_ao;

/* Constructors */
void mmi_ctor(void);
void prog_ctor(void);
void shutter_ctor(void);
void servo_ctor(void);

#endif // __CAMCONTROL_H__
