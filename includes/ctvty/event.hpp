#ifndef Event_hh__
# define Event_hh__

#include <vector>

/*
 * here's what's important in this .hpp
 */
namespace ctvty {
  namespace event {
    namespace parameters {
      class value;
      using values = std::vector<value*>;

      template<typename ... _values>
      values PackValues(_values...);
    };
  };
};

namespace ctvty {
  namespace event {
    
    namespace parameters {
      /*
       * event values packers
       */
      template<typename _type>
      class intern_value;

      class value {
      public:
	virtual std::string	getType() = 0;
	
      public:
	template<typename _type>
	static value*		create(_type _value) {
	  return (new intern_value<_type>(_value));
	}

	template<typename _type>
	_type			as() {
	  return (dynamic_cast< intern_value<_type>* >(this)->get());
	}

	template<typename _type>
	bool			is() {
	  return (dynamic_cast< intern_value<_type>* >(this) != nullptr);
	}
      };

      template<typename _type>
      class intern_value : public value{
      private:
	_type		_value;

      public:
	_type		get() { return _value; }

      public:
	intern_value(_type _v) : _value(_v) {}
      };

      template<typename ... _values>
      values PackValues(_values ... vs) {
	values retval = {
	  (value::create<_values>(vs))...,
	}
	return (retval);
      }

    };

    /*
     * event Receiver
     */
    template<typename ... parameters>
    class intern_receiver;

    class receiver {
    public:
      virtual void	operator() (parameter::values _params) = 0;

    public:
      template<typename ... parameters>
      static receiver* create(std::function<void (parameters...)> _fn) {
	return (new intern_receiver< parameters ... >(_fn));
      }
    };

    template <int N, typename... T>
    struct typeAt;

    template <typename T0, typename... T>
    struct typeAt<0, T0, T...> {
      typedef T0 type;
    };

    template <int N, typename T0, typename... T>
    struct typeAt<N, T0, T...> {
      typedef typename typeAt<N-1, T...>::type type;
    };


    template<unsigned int N>
    struct unfolder;

    template<unsigned int N>
    struct unfolder {
      template<typename... args, typename ...final>
      static void apply(parameter::values values, std::function<void (args...)> fn, final ... sended) {
	if (!values[sizeof ... (args) - N]->is< typename typeAt<sizeof ... (args) - N, args...>::type >())
	  throw std::runtime_error("mismatch type on call");
	unfolder<N - 1>::apply(values, fn, sended ..., values[sizeof ... (args) - N]->as< typename typeAt<sizeof ... (args) - N, args...>::type >());
      }
    };

    template<>
    struct unfolder <0> {
      template<typename ... args, typename ... final>
      static void apply(parameter::values values, std::function<void (args...)> fn, final ... sended) {
	fn( sended ... );
      }
    };


    template<typename ... parameters>
    class intern_receiver : public receiver{
    private:
      std::function<void (parameters...)>	_fn;

    public:
      intern_receiver(std::function<void(parameters...)> fn) : _fn(fn) {}

    public:
      void operator() (parameter::values _values) {
	if (_values.size() < sizeof ... (parameters))
	  throw std::runtime_error("not enough operands");
	unfolder<sizeof ... (parameters)>::apply(_values, _fn);
      }
    };

  };
};

#endif
