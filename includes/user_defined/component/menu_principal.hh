#ifndef MenuPrincipal_hh__
# define MenuPrincipal_hh__

#include "ctvty/monobehaviour.hpp"
#include "ctvty/component/hud.hh"

namespace user_defined {
  namespace component {

    class MenuPrincipal : public ctvty::MonoBehaviour<MenuPrincipal>{
    public:
			MenuPrincipal(const serialization::Archive&);
      void		Serialize(serialization::Archive&) const;

    public:
      void		Play(ctvty::component::Hud*);
      void		OverPlay(ctvty::component::Hud*);
      void		OnOverPlay(ctvty::component::Hud*);

      void		Options(ctvty::component::Hud*);
      void		OverOptions(ctvty::component::Hud*);
      void		OnOverOptions(ctvty::component::Hud*);

      void	        OverLoadGame(ctvty::component::Hud*);
      void		OnOverLoadGame(ctvty::component::Hud*);

      void		OverCredit(ctvty::component::Hud*);
      void		OnOverCredit(ctvty::component::Hud*);

      void		OverExit(ctvty::component::Hud*);
      void		OnOverExit(ctvty::component::Hud*);
      void		Exit(ctvty::component::Hud*);
    };

  };
};

#endif
