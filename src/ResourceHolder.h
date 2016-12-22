#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>

enum class FontID {
  Sansation
};

template <typename Identifier, typename Resource>
class ResourceHolder {
  public:
    void load(Identifier id, std::string const & filename) {
      std::unique_ptr<Resource> resource(new Resource());
      if (!resource->loadFromFile(filename))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);

      auto inserted = m_resourceMap.insert(std::make_pair(id, std::move(resource)));
      assert(inserted.second);
    }

    Resource& get(Identifier id) {
      auto found = m_resourceMap.find(id);
      assert(found != m_resourceMap.end());
      return *found->second;
    }

   const Resource& get(Identifier id) const {
     auto found = m_resourceMap.find(id);
     assert(found != m_resourceMap.end());
     return *found->second;
   }

  private:
    std::unordered_map<Identifier, std::unique_ptr<Resource>> m_resourceMap;
};

