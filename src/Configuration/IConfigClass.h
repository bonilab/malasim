#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

class Config;

class IConfigClass {
public:
    virtual ~IConfigClass() = default;
    virtual void process_config() = 0;
};

#endif  // CONFIG_DATA_H

