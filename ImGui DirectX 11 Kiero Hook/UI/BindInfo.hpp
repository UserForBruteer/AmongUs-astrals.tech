enum class BindMode {
    Toggle,
    Hold
};

struct BindInfo {
    bool* target;
    BindMode mode;
    bool last_hold_state = false;
    const char* name;
};
