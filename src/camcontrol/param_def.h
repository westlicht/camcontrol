/** @file param_def.h
 *
 * Parameter definitions.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

/* Parameter classes */
PARAM_CLASS(pc_focal_length, UINT16, 100, 5000, 1, 10, print_mm)
PARAM_CLASS(pc_sensor_dim, UINT16, 100, 500, 1, 10, print_mm)
PARAM_CLASS(pc_crop, UINT16, 100, 500, 1, 10, print_crop)
PARAM_CLASS(pc_servo_range, UINT16, 0, 800, 1, 10, print_decimal)
PARAM_CLASS(pc_backlight, UINT16, 5, 300, 1, 5, print_seconds)
PARAM_CLASS(pc_deg_360, UINT16, 0, 360, 1, 10, print_deg)
PARAM_CLASS(pc_deg_180, UINT16, 0, 180, 1, 10, print_deg)
PARAM_CLASS(pc_shutter_mode, UINT8, 0, NUM_SHUTTER_MODES - 1, 1, 1, print_shutter_mode)
PARAM_CLASS(pc_exposure, UINT8, 0, NUM_EXPOSURE_INFOS - 1, 1, 1, print_exposure)
PARAM_CLASS(pc_hdr_shots, UINT8, 2, 10, 1, 5, print_decimal)
PARAM_CLASS(pc_trigger_delay, UINT16, 10, 500, 1, 10, print_trigger_delay)
PARAM_CLASS(pc_duration, UINT32, 1, 86400, 1, 60, print_duration)
PARAM_CLASS(pc_overlap, UINT16, 10, 500, 1, 100, print_overlap)

/* Camera parameters */
PARAM(p_focal_length, &pc_focal_length, &pd.camera.focal_length, 300)
PARAM(p_sensor_width, &pc_sensor_dim, &pd.camera.sensor_width, 149)
PARAM(p_sensor_height, &pc_sensor_dim, &pd.camera.sensor_height, 223)
PARAM(p_crop, &pc_crop, &pd.camera.crop, 100)

/* Servo parameters */
PARAM(p_servo_min_x, &pc_servo_range, &pd.servo.min_x, 50)
PARAM(p_servo_max_x, &pc_servo_range, &pd.servo.max_x, 750)
PARAM(p_servo_min_y, &pc_servo_range, &pd.servo.min_y, 100)
PARAM(p_servo_max_y, &pc_servo_range, &pd.servo.max_x, 700)

/* Misc parameters */
PARAM(p_misc_backlight, &pc_backlight, &pd.misc.backlight, 15)

/* Shutter parameters */
PARAM(p_shutter_mode, &pc_shutter_mode, &pd.shutter.mode, 0)
PARAM(p_shutter_time, &pc_exposure, &pd.shutter.time, 0)
PARAM(p_shutter_long_time, &pc_duration, &pd.shutter.long_time, 60)
PARAM(p_hdr_time1, &pc_exposure, &pd.shutter.hdr_time1, 0)
PARAM(p_hdr_time2, &pc_exposure, &pd.shutter.hdr_time2, 5)
PARAM(p_hdr_shots, &pc_hdr_shots, &pd.shutter.hdr_shots, 5)
PARAM(p_trigger_delay, &pc_trigger_delay, &pd.shutter.post_delay, 20)

/* Single shot parameters */
PARAM(p_center_x, &pc_deg_360, &pd.single.center_x, 180)
PARAM(p_center_y, &pc_deg_180, &pd.single.center_y, 90)

/* Spherical pan parameters */
PARAM(p_spherical_overlap, &pc_overlap, &pd.spherical.overlap, 100)

/* Giga pan parameters */
PARAM(p_giga_start_x, &pc_deg_360, &pd.giga.start_x, 130)
PARAM(p_giga_start_y, &pc_deg_180, &pd.giga.start_y, 60)
PARAM(p_giga_end_x, &pc_deg_360, &pd.giga.end_x, 230)
PARAM(p_giga_end_y, &pc_deg_180, &pd.giga.end_y, 120)
PARAM(p_giga_overlap, &pc_overlap, &pd.giga.overlap, 100)

/* Timelapse parameters */
PARAM(p_timelapse_rate, &pc_duration, &pd.timelapse.rate, 60)
