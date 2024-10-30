namespace odom {

    //3 encoders vs 2 encoders + IMU
    //each wheel rotational offset 

    //TODO: 

    /**
     * @brief Struct to hold the current position of the robot
     *  NOTE: The field resides in the 1st quadrant with the origin at the bottom left corner
     *      of the field with respect to the starting position
     * @param x the x position of the robot in inches
     * @param y the y position of the robot in inches
     * @param theta the current heading (unbounded) of the robot in degrees
     */
    struct coord {
        double x;
        double y;
        double theta;
    };
 

    struct offsets {
        double BL_offset; 
        double BR_offset; 
        double FL_offset; 
        double FR_offset; 
    };

    /**
     * @brief Enum to hold the type of odom configuration
     * @option TWO_ENCODER_IMU: Two encoders in x and y config and an IMU
     * @option THREE_ENCODER: Three encoders two on the x and one on the y
     * @option THREE_ENCODER_IMU: Three encoders two on the x and one on the y and an IMU
     */
    enum odomType {
        TWO_ENCODER_IMU,
        THREE_ENCODER,
        THREE_ENCODER_IMU // Preferred
    };

    /**
     * @brief Reset and initialize the encoder values and IMU
     * @return true if success, false if failure which will require a reboot
     */
    bool init(odomType type);

    /**
     * @brief Set the starting position of the robot.
     *  NOTE: Should be called before starting the odom task otherwise the robot will start at the origin
     * 
     * @param x the x position of the robot in inches
     * @param y the y position of the robot in inches
     * @param theta the current heading (unbounded) of the robot in degrees
     */
    void setPos(double x, double y, double theta);

    /**
     * @brief Start the odom task in its own thread
     */
    void start();

    /**
     * @brief Stop the odom task
     */
    void stop();

    /**
     * @brief Get the current position of the robot
     * @return the current position of the robot in the struct coord
     *  EXAMPLE: int x_pos = getPos().x;
     */
    coord getPos();
} 