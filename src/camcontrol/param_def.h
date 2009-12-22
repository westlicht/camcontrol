
#if 0
struct param_data {
	uint16_t focal_length;		/**< Focal length (mm * 0.1) */
	uint16_t sensor_width;		/**< Sensor width (mm * 0.1) */
	uint16_t sensor_height;		/**< Sensor height (mm * 0.1) */
	uint16_t fov_h;				/**< Horizontal field of view (deg * 0.01) */
	uint16_t fov_v;				/**< Vertical field of view (deg * 0.01) */
	uint16_t pan_width;			/**< Panorama width (deg) */
	uint16_t pan_height;		/**< Panorama height (deg) */
	uint8_t shutter_mode;		/**< Shutter mode */
	uint8_t hdr_time1;			/**< Min exposure time for HDR series */
	uint8_t hdr_time2;			/**< Max exposure time for HDR series */
	uint16_t hdr_shots;			/**< Number of shots per HDR series */
	uint16_t trigger_delay;		/**< Trigger delay between shots in s * 0.01 */
	uint32_t timelapse_rate;	/**< Timelapse trigger rate in s */
};
#endif

PARAM_CLASS(pc_focal_length, UINT16, 100, 500, 200, 1, 10, print_mm)
PARAM_CLASS(pc_sensor_dim, UINT16, 100, 500, 200, 1, 10, print_mm)
//PARAM_CLASS(pc_deg_180, UINT16, 0, 180, 90, 1, 10, print_mm)
//PARAM_CLASS(pc_deg_360, UINT16, 0, 180, 90, 1, 10, print_mm)
PARAM_CLASS(pc_shutter_mode, UINT8, 0, NUM_SHUTTER_MODES - 1, 0, 1, 1, print_shutter_mode)
PARAM_CLASS(pc_shutter_time, UINT8, 0, NUM_SHUTTER_TIMES - 1, 0, 1, 1, print_shutter_time)
PARAM_CLASS(pc_hdr_shots, UINT8, 2, 10, 5, 1, 5, print_decimal)
PARAM_CLASS(pc_trigger_delay, UINT16, 10, 500, 100, 1, 10, print_trigger_delay)
PARAM_CLASS(pc_timelapse_rate, UINT32, 1, 100000, 60, 1, 60, print_timelapse_rate)


PARAM(p_focal_length, &pc_focal_length, &pd.focal_length)
PARAM(p_sensor_width, &pc_sensor_dim, &pd.sensor_width)
PARAM(p_sensor_height, &pc_sensor_dim, &pd.sensor_height)
PARAM(p_shutter_mode, &pc_shutter_mode, &pd.shutter_mode)
PARAM(p_hdr_time1, &pc_shutter_time, &pd.hdr_time1)
PARAM(p_hdr_time2, &pc_shutter_time, &pd.hdr_time2)
PARAM(p_hdr_shots, &pc_hdr_shots, &pd.hdr_shots)
PARAM(p_trigger_delay, &pc_trigger_delay, &pd.trigger_delay)
PARAM(p_timelapse_rate, &pc_timelapse_rate, &pd.timelapse_rate)
