//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_RESOURCE_H
#define SIMPLE_P2P_RESOURCE_H

#include <vector>
#include <string>
#include <GeneralTypes.h>
#include <shared_mutex>

#define SEGMENT_SIZE 1024 // 1kb

namespace simpleP2P {

class Host; //!< Forward declaration

/**
 * Class contains file information and points to nodes with file possesion
 */
class Resource {
public:
  /**
   * Constructor
   * @param name filename
   * @param size filesize
   * @param path filepath, default is "./"
   */
  Resource(std::string name, Uint64 size, std::string path = "./");

  /**
   * Constructor makes resource from header
   * @param resource_header Resource header
   */
  Resource(std::vector<Uint8> resource_header);

  Resource(const Resource &other) : Resource(other.name, other.size, other.path) {}

  /**
   * Generates Resource header
   * @return Resource header
   */
  std::vector<Uint8> generate_resource_header();

  /**
   * Determines if resource is possesed by Host
   * @param host Host
   * @return true if resource is possessed by host
   */
  bool has_host(Host host);

  /**
   * Calculates and returns segment count
   * @return segment count
   */
  Uint16 calc_segments_count() const {
    std::shared_lock lock(resource_mutex);
    if (size % SEGMENT_SIZE)
      return 1 + (size / SEGMENT_SIZE);
    else
      return size / SEGMENT_SIZE;
  }

  /**
   * Function used to set invalidated flag.
   * To allow references on resource outside database to gather information about revoke
   */
  void set_revoked();

  bool isInvalidated();

  /**
   * Getter for file size
   * @return file size
   */
  Uint64 getSize() const;

  /**
   * Getter for file name
   * @return file name
   */
  const std::string &getName() const;

  /**
   * Getter for file path
   * @return file path
   */
  const std::string &getPath() const;

  /**
   * Operator == checks file size and name for equality
   * @param other other
   * @return true if equal
   */
  bool operator==(const Resource &other) const;

  /**
   * Operator != checks file size and name for equality
   * @param other other
   * @return true if not equal
   */
  bool operator!=(const Resource &other) const;

  std::vector<std::weak_ptr<Host>> get_hosts();

  void remove_host(const Host &host);

  void add_host(const std::shared_ptr<Host> &host);
private:
  Uint64 size;                            //!< file size
  std::string name;                       //!< file name
  /*atrribs not checked for equality*/
  std::shared_mutex mutable resource_mutex;//!< mutex for mutual access synchronisation
  bool invalidated;                       //!< indicates that resource has been revoked
  std::string path;                       //!< file path
  std::vector<std::weak_ptr<Host>> hosts_in_possession;  //!< Host in possession of the Resource
};
}

#endif //SIMPLE_P2P_RESOURCE_H
