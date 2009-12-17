#ifndef __CAMCONTROL_H__
#define __CAMCONTROL_H__

/** Active object signals */
enum signals {						/**< Description			Parameter		*/

	SIG_KEY_PRESS = Q_USER_SIG,		/**< Key pressed			Key index		*/
	SIG_KEY_RELEASE,				/**< Key release			Key index		*/
	SIG_ENCODER,					/**< Encoder turned			Direction		*/

};

/* Active objects */
extern struct mmi_ao mmi_ao;

/* Constructors */
void mmi_ctor(void);

#endif // __CAMCONTROL_H__
