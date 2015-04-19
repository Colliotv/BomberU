#include <iostream>
#include "serialization/serializable.hh"
#include "filesystem/directory.hh"
#include "ctvty/gameObject.hpp"
#include "ctvty/application.hh"
#include "ctvty/event/clock.hh"
#include "ctvty/assets/assets.hh"

void	recursiveList(filesystem::Directory& directory) {
  std::list<filesystem::Directory>	subList;

  if (directory) {
    std::cout << directory.GetPath() << ":" << std::endl;
    for (const filesystem::File& file : directory) {
      std::cout << file.GetName() << " ";
      if (file.GetName() != "." && file.GetName() != ".." && file.IsDirectory())
	subList.emplace_back(directory.GetPath() + '/' + file.GetName());
    }
    std::cout << std::endl << std::endl;
    std::for_each(subList.begin(), subList.end(),
		  &recursiveList);
  }
}

int main(int ac, char** av) {
  ctvty::asset::Cache<ctvty::GameObject>
					*save_cache;

  ctvty::asset::Assets			assets("assets");
  ctvty::asset::Assets			save_assets(std::move(assets.GetAssets("save")));

  ctvty::GameObject			*Army = nullptr;

  {
    bool	initialization_succes;
    if (ac == 2)
      initialization_succes = ctvty::Application::Initialize(av[1]);
    else
      initialization_succes = ctvty::Application::Initialize("assets");
    if (!initialization_succes)
      return (-1);
  }

  if (1)
    {
      Army  = new ctvty::GameObject("Army", "army", nullptr,
				    new ctvty::utils::Vector3D(0.f, 0.f, 1.f), new ctvty::utils::Quaternion(0.f, 0.f, 0.f, 1.f));
      Army->AddChild(new ctvty::GameObject("Soldier1", "soldier", nullptr,
					   new ctvty::utils::Vector3D(0.f, 0.f, 1.f), new ctvty::utils::Quaternion(0.f, 0.f, 0.f, 1.f)));
      Army->AddChild(new ctvty::GameObject("Soldier1", "soldier", nullptr,
					   new ctvty::utils::Vector3D(0.f, 0.f, 1.f), new ctvty::utils::Quaternion(0.f, 0.f, 0.f, 1.f)));
      Army->AddChild(new ctvty::GameObject("Soldier2", "soldier", nullptr,
					   new ctvty::utils::Vector3D(0.f, 0.f, 1.f), new ctvty::utils::Quaternion(0.f, 0.f, 0.f, 1.f)));
      Army->AddChild(new ctvty::GameObject("Soldier2", "soldier", nullptr,
					   new ctvty::utils::Vector3D(0.f, 0.f, 1.f), new ctvty::utils::Quaternion(0.f, 0.f, 0.f, 1.f)));
    }

  if (1)
    {
      std::cout << "assets are cached" << std::endl;
      save_cache = new ctvty::asset::Cache<ctvty::GameObject>([](ctvty::asset::Asset& file)->bool {
	  bool	pass = file.GetFile().GetName()
	    .substr(file.GetFile().GetName().size()-sizeof("json") + 1, sizeof("json")) == "json";
	  pass = pass && (file.GetFile().GetName() != "ArmyArray.json");
	  if (pass)
	    file.SetDeleter([](serialization::Serializable* obj) {
		if (dynamic_cast<ctvty::Object*>(obj) != nullptr)
		  ctvty::Object::Destroy(dynamic_cast<ctvty::Object*>(obj));
	      });
	  return pass;
	});
      save_assets.Cache(save_cache);
    }

  if (1)
    {
      std::cout << "Army will be saved" << std::endl;
      save_assets.GetAsset("Army.json").Save(Army);
    }

  if (0)
    {
      std::cout << "some copy are made" << std::endl;
      std::shared_ptr<ctvty::GameObject>	ArmyCopy1 = save_assets.GetAsset("Army.json").LoadAs<ctvty::GameObject>();
      std::shared_ptr<ctvty::GameObject>	ArmyCopy2 = save_assets.GetAsset("Army.json").LoadAs<ctvty::GameObject>();
      std::shared_ptr<ctvty::GameObject>	ArmyCopy3 = save_assets.GetAsset("Army.json").LoadAs<ctvty::GameObject>();
    }

  if (0)
    {
      filesystem::Directory root(".");
      recursiveList(root);
    }

  if (1)
    {
      std::cout << "call to Destroy" << std::endl;
      ctvty::Object::Destroy(Army);
    }

  std::cout << "delete cache" << std::endl;
  delete save_cache;
}
