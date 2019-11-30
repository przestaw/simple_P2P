#ifndef __CLIRESOURCESERVICE_H__
#define __CLIRESOURCESERVICE_H__

#include <string>

class CLIResourceService
{
private:
    void copy_file(std::string resource, char *path);
    void delete_files(std::string resource);

public:
    CLIResourceService();
    ~CLIResourceService();
    void add(std::string resource, char *args[]);
    void remove(std::string resource, char *args[]);
    void revoke(std::string resource, char *args[]);
    void get_local(); // TODO return resources
    void get_remote();
};

#endif // __CLIRESOURCESERVICE_H__
