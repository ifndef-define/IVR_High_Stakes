class Clamp
{
private:
    bool clampState;
    // true = clamp is closed
    // false = clamp is open
public:
    void clampOpen();
    void clampClose();
    void clampToggle();
    void clampGetState();
};


