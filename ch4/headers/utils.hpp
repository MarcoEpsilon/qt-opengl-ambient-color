#ifndef __UTILS_HPP__
#define __UTILS_HPP__
class QString;
class Utils {
public:
    explicit Utils();
    ~Utils();
    enum SliderGrooveType {
        GROOVE_RED,
        GROOVE_GREEN,
        GROOVE_BLUE
    };
    static QString readFile(const char* fileName);
};
#endif /*__UTILS_HPP__*/