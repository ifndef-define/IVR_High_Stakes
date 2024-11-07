#pragma once
#include "main.h"
#include "common_code/generic_rotation.h"


class Generic_Rotation_Digikey : public Generic_Rotation {
    public:
        Generic_Rotation_Digikey(pros::ADIEncoder& rot_sensor, double wheel_radius, double offset_from_rot_axis_meters = 0.0);
        double get_meters_travelled();
        double get_raw_data();
        void initialize_sensor();
    private:
        pros::ADIEncoder& rotation_sensor;
        double get_delta_rotation(); 
        double last_value;
        const double kTICKS_PER_REVOLUTION = 5000.0; // CPR (counts per revolution) based on digikey
        // documentation here: https://www.cuidevices.com/product/resource/amt10e-v.pdf
        // Since our CPR is 5000 our PPR is 1250 which means our DIP switch is set to 1010

};