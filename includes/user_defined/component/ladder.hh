#ifndef LADDER_HH_
# define LADDER_HH_

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/hud.hh"

namespace user_defined
{
  namespace component
  {

    class ScoreLog : public serialization::Serializable
    {
    public:
      ScoreLog(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      int& operator[](const std::string &);

    private:
      std::map<std::string, int>		log;
    };

    class Ladder : public ctvty::MonoBehaviour<Ladder>
    {
    public:
      Ladder(const serialization::Archive&);
      void	Serialize(serialization::Archive&) const;

    public:
      void Awake();
      void OnDestroy();
      void Scored(const std::string&);

    private:
      std::string		path;
      std::shared_ptr<ScoreLog>	log;
    };
  };
};

#endif
