class Pnuematic
{
private:
    bool State;
    // true = pnuematic is closed
    // false = pnuematic is open
public:
    void open();
    void close();
    void toggle();
    void getState();
};


