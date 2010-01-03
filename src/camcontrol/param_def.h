/** @file param_def.h
 *
 * Parameter definitions.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

/* Parameter classes */
PARAM_CLASS(pc_focal_length, UINT16, 100, 5000, 1, 10, print_mm)
PARAM_CLASS(pc_sensor_dim, UINT16, 100, 500, 1, 10, print_mm)
PARAM_CLASS(pc_deg_360, UINT16, 0, 360, 1, 10, print_deg)
PARAM_CLASS(pc_deg_180, UINT16, 0, 180, 1, 10, print_deg)
PARAM_CLASS(pc_shutter_mode, UINT8, 0, NUM_SHUTTER_MODES - 1, 1, 1, print_shutter_mode)
PARAM_CLASS(pc_shutter_time, UINT8, 0, NUM_SHUTTER_TIMES - 1, 1, 1, print_shutter_time)
PARAM_CLASS(pc_hdr_shots, UINT8, 2, 10, 1, 5, print_decimal)
PARAM_CLASS(pc_trigger_delay, UINT16, 10, 500, 1, 10, print_trigger_delay)
PARAM_CLASS(pc_timelapse_rate, UINT32, 1, 100000, 1, 60, print_timelapse_rate)
PARAM_CLASS(pc_overlap, UINT16, 10, 500, 1, 100, print_overlap)

/* Parameters */
PARAM(p_focal_length, &pc_focal_length, &pd.focal_length, 500)
PARAM(p_sensor_width, &pc_sensor_dim, &pd.sensor_width, 149)
PARAM(p_sensor_height, &pc_sensor_dim, &pd.sensor_height, 223)
PARAM(p_shutter_mode, &pc_shutter_mode, &pd.shutter_mode, 0)
PARAM(p_shutter_time, &pc_shutter_time, &pd.shutter_time, 0)
PARAM(p_hdr_time1, &pc_shutter_time, &pd.hdr_time1, 0)
PARAM(p_hdr_time2, &pc_shutter_time, &pd.hdr_time2, 0)
PARAM(p_hdr_shots, &pc_hdr_shots, &pd.hdr_shots, 5)
PARAM(p_trigger_delay, &pc_trigger_delay, &pd.trigger_delay, 100)
PARAM(p_timelapse_rate, &pc_timelapse_rate, &pd.timelapse_rate, 60)
PARAM(p_center_x, &pc_deg_360, &pd.center_x, 180)
PARAM(p_center_y, &pc_deg_180, &pd.center_y, 90)

/* Giga pan */
PARAM(p_giga_start_x, &pc_deg_360, &pd.giga.start_x, 130)
PARAM(p_giga_start_y, &pc_deg_180, &pd.giga.start_y, 60)
PARAM(p_giga_end_x, &pc_deg_360, &pd.giga.end_x, 230)
PARAM(p_giga_end_y, &pc_deg_180, &pd.giga.end_y, 120)
PARAM(p_giga_overlap, &pc_overlap, &pd.giga.overlap, 100)
