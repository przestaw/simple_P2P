#ifndef SIMPLE_P2P_CLIRESOURCESERVICE_H
#define SIMPLE_P2P_CLIRESOURCESERVICE_H

#include <string>
namespace simpleP2P
{

class CLIResourceService
{
public:
    CLIResourceService();
    ~CLIResourceService();
    void add(std::string resource);
    void remove(std::string resource);
    void revoke(std::string resource);
    void get_local(); // TODO return resources
    void get_remote();
};
} // namespace simpleP2P

#endif // SIMPLE_P2P_CLIRESOURCESERVICE_H
