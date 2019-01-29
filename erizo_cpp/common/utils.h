#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdlib.h>
#include <unistd.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/regex.hpp>

#include <json/json.h>

class Utils
{
  public:
    static int initPath()
    {
        char buf[256] = {0};
        char filepath[256] = {0};
        char cmd[256] = {0};
        FILE *fp = NULL;

        sprintf(filepath, "/proc/%d", getpid());
        if (chdir(filepath) < 0)
            return 1;

        snprintf(cmd, 256, "ls -l | grep exe | awk '{print $11}'");
        if ((fp = popen(cmd, "r")) == nullptr)
        {
            return 1;
        }

        if (fgets(buf, sizeof(buf) / sizeof(buf[0]), fp) == nullptr)
        {
            pclose(fp);
            return 1;
        }

        std::string path = buf;
        size_t pos = path.find_last_of('/');
        if (pos != path.npos)
            path = path.substr(0, pos);

        if (chdir(path.c_str()) < 0)
            return 1;
        return 0;
    }

    static std::string getUUID()
    {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return boost::uuids::to_string(uuid);
    }

    static std::string dumpJson(const Json::Value &root)
    {
        Json::FastWriter writer;
        return writer.write(root);
    }
};

#endif