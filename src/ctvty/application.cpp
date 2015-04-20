#include "ctvty/application.hh"
#include "ctvty/event/clock.hh"

namespace ctvty {

  REGISTER_FOR_SERIALIZATION(ctvty, Application);
  REGISTER_FOR_SERIALIZATION(ctvty::Application, Scene);

  Application::		Application(const serialization::Archive& __serial)
    : leave_state(false), delayed_quit(nullptr) {
    float	fps;

    __serial["scenes"] & scenes;
    __serial["load"] & base_scene;
    if (__serial.exist("fps"))
      __serial["fps"] & fps;
    else
      fps = 30;
    ctvty::event::Clock::GetClock().SetMaxFramePerSeconds(fps);
  }

  Application::		~Application() {
    for (Scene* scene : scenes)
      delete scene;
    delete assets;
  }

  void			Application::Serialize(serialization::Archive& __serial_instance) {
    float	fps = 30;
    SERIALIZE_OBJECT_AS(ctvty::Application, __serial_instance);
    __serial["scenes"] & scenes;
    __serial["fps"] & fps;
    __serial["load"] & base_scene;
  }

  bool			Application::Initialize(const std::string& location) {
    Application*&		app = GetApplication();
    ctvty::asset::Assets*	assets = new ctvty::asset::Assets(location);
    serialization::Serial*	serial;

    {//serialization asset function
      serialization::StoreFunction("asset",
				   [&assets] (const serialization::Serial& __serial) -> serialization::Serial* {
	  std::string path;
	  __serial & path;
	  
	  serialization::Serial* product =
	    serialization::Serial::InstantiateFromFile(assets->GetAsset(path).GetFile().GetPath());
	  return product;
	});
    }

    {//application deserialization
      if (!(assets->GetAsset("application.json").GetFile())) {
	std::cerr << "application error: application.json not found, the application will quit" << std::endl;
	return false;
      }
      serial = serialization::Serial::InstantiateFromFile(assets->GetAsset("application.json")
							  .GetFile().GetPath());
      app = (Application*)serialization::Serializable::Instantiate(*serial);
      app->assets = assets;
      delete serial;
    }

    return true;
  }

  void			Application::Start() {
    Application*	app = GetApplication();
    LoadScene(app->base_scene);
    ctvty::event::Clock::GetClock().Start();
  }

  void			Application::LoadScene(const std::string& name) {
    Application*	app = GetApplication();

    std::list<GameObject*> fathers_copy(ctvty::GameObject::accessParentsGameObjects());
    ctvty::GameObject::accessParentsGameObjects().clear();
    for (GameObject* gameObject : fathers_copy)
      Object::Destroy(gameObject);
    for (Scene* scene : app->scenes)
      if (scene->GetName() == name) {
	scene->Instantiate();
	break;
      }
  }

  void			Application::CancelQuit() {
    Application*	app = GetApplication();
    if (app->delayed_quit != nullptr) {
      delete app->delayed_quit;
      app->delayed_quit = nullptr;
    }
    app->leave_state = false;
  }

  void			Application::Quit() {
    Application*	app = GetApplication();

    app->leave_state = true;
    for (GameObject* gameObject : ctvty::GameObject::accessParentsGameObjects())
      gameObject->BroadcastMessage("OnApplicationQuit");
    if (!app->leave_state)
      return ;
    event::Clock::GetClock().End();
    std::list<GameObject*> fathers_copy(ctvty::GameObject::accessParentsGameObjects());
    ctvty::GameObject::accessParentsGameObjects().clear();
    for (GameObject* gameObject : fathers_copy)
      Object::Destroy(gameObject);
    delete app;
  }

  void			Application::Quit(float time) {
    void		(*quit)() = &Application::Quit;
    Application*	app = GetApplication();

    app->leave_state = true;
    new ctvty::event::DelayedAction(quit, time);
  }

  asset::Assets&	Application::Assets() { return *(GetApplication()->assets); }

  Application*&		Application::GetApplication() { static Application* app = nullptr; return app; }

  Application::Scene::	Scene(const serialization::Archive& __serial) {
    __serial["objects"] & objects;
    __serial["name"] & name;
  }

  Application::Scene::	~Scene() {
    for (GameObject* prefab : objects)
      Object::Destroy(prefab);
  }

  void			Application::Scene::Serialize(serialization::Archive& __serial_instance) {
    SERIALIZE_OBJECT_AS(ctvty::Application::Scene, __serial_instance);
    __serial["objects"] & objects;
    __serial["name"] & name;
  }

  std::string		Application::Scene::GetName() const {
    return name;
  }

  void			Application::Scene::Instantiate() const {
    for (GameObject* baseObject : objects)
      Object::Instantiate(baseObject);
    std::cout << ctvty::GameObject::accessParentsGameObjects().size() << std::endl;
  }
};
