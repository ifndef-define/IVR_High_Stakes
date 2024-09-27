# IVR_High_Stakes
Illini VEX Robotics' source code and libraries for the 2024-2025 Vex High Stakes competition season. Developed using [PROS-Code](https://pros.cs.purdue.edu/) for V5 in [Visual Studio Code](https://code.visualstudio.com/).

## Important
Do NOT share this repository or any code within this repository with anyone outside this organization. After the season end, we may publish this repository at our discretion.

### Include Structure
Refer to the following image when including files into the `.h` and `.cpp` files
**NOTE:**
- Robot specific files refers to the files that contain specific subsystems like the intake
- Robot common files refers to files that controls or runs code that encompasses the entire robot like `main.cpp` or the `controls.cpp` 
![alt text](github_img/include_structure.png)

### Note For Intellisense Errors
- Cpp settings set intellisence to default
- add config settings to the c_cpp_properties
- if g++ compiler set that in c_cpp_properties
- clean project, then do `pros make all`