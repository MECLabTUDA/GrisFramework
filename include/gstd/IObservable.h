#pragma once

/**
* Contains the class templates IObservable.
*
* @author David Kügler
* @date 25-04-2016
*
* Based on https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
*/

#include <functional>  // for std::function
#include <utility>     // for std::forward and std::move
#include <map>
#include <vector> 

namespace gris {
namespace gstd {

	/**
	* Class IObservable implements the interface for the Observer pattern.
	* The Observers are (void(Args...)) function-Pointers, Event can be any type, e.g. custom enums
	*
	* Based on https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
	*/
	template <typename Event, typename...Args>
	class IObservable
	{
	public:
    typedef void Functor(Args...);
    typedef std::function<Functor> Observer;
    typedef Observer* IdentifierType;

  public:
    IObservable() = default;
    IObservable(const IObservable& other) = default;
    IObservable(IObservable&& other)
    {
      mObservers = other.mObservers;
    }

  public:
    void swap(IObservable& rhs)
    {
      mObservers.swap(rhs.mObservers);
    }

  public:
    static constexpr char* s_name() { return "IObservable Template"; }
    virtual const char* name() const { return s_name(); }
    virtual bool isA(const std::string& classname) const { return classname == s_name(); }
  public:
    virtual IdentifierType registerObserver(const Event& event, Observer&& observer)
    {
      auto& v = mObservers[event];
      v.push_back(std::forward<Observer>(observer));
      return &v.back();
    }

    virtual IdentifierType registerObserver(Event&& event, Observer&& observer)
    {
      auto& v = mObservers[std::move(event)];
      v.push_back(std::forward<Observer>(observer));
      return &v.back();
    }

    virtual IdentifierType registerObserver(const Event& event, Observer& observer)
    {
      auto& v = mObservers[event];
      v.push_back(observer);
      return &v.back();
    }

    virtual IdentifierType registerObserver(Event&& event, Observer& observer)
    {
      auto& v = mObservers[std::move(event)];
      v.push_back(observer);
      return &v.back();
    }

    void unregisterObserver(const Event& event, const IdentifierType identifier)
    {
      auto& v = mObservers[event];
      for (auto it = v.begin(); it != v.end(); ++it)
        if (&*it == identifier)
        {
          v.erase(it);
          return;
        }
    }

    void unregisterObserver(Event&& event, const IdentifierType identifier)
    {
      auto& v = mObservers[std::move(event)];
      for (auto it = v.begin(); it != v.end(); ++it)
        if (&*it == identifier)
        {
          v.erase(it);
          return;
        }
    }

		virtual void notify(const Event& event, typename std::remove_reference<Args>::type &&... args) const
		{
			for (const auto& obs : mObservers.at(event)) obs(std::forward<Args>(args)...);
		}

		virtual void notify(const Event& event, typename std::remove_reference<Args>::type &... args) const
		{
			for (const auto& obs : mObservers.at(event)) obs(args...);
		}

		size_t observerCount(const Event& event) const
		{
			auto it = mObservers.find(event);
			if (it == mObservers.end()) return 0;
			else return it->second.size();
		}

		size_t observerCount(Event&& event) const
		{
			auto it = mObservers.find(std::move(event));
			if (it == mObservers.end()) return 0;
			else return it->second.size();
		}
	private:
		std::map<Event, std::vector<Observer>> mObservers;
	};


} // namespace muk
} // namespace gris
