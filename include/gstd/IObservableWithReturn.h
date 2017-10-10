#pragma once

/**
* Contains the class templates IObservableWithReturn.
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
namespace muk {

	/**
	* Class IObservableWithReturn implements the interface for the Observer pattern including a way to handle returns of multiple Observers.
	* The Observers are (TypeOfReturn(Args...)) function-Pointers, Event can be any type, e.g. custom enums
	*
	* Based on https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/
	*/
	template <typename Event, typename TypeOfReturn, typename...Args>
	class IObservableWithReturn
	{
  public:
    typedef TypeOfReturn ReturnType;
    typedef ReturnType Functor(Args...);
    typedef std::function<Functor> Observer;

	public:
		void registerObserver(const Event& event, Observer&& observer)
		{
			mObservers[event].push_back(std::forward<Observer>(observer));
		};

		void registerObserver(Event&& event, Observer&& observer)
		{
			mObservers[std::move(event)].push_back(std::forward<Observer>(observer));
		};

		std::vector<ReturnType> notify(const Event& event, typename std::remove_reference<Args>::type &&... args) const
		{
			std::vector<ReturnType> vec;
			for (const auto& obs : mObservers.at(event)) vec.push_back(obs(std::forward<Args>(args)...));
			return vec;
		};

		std::vector<ReturnType> notify(const Event& event, typename std::remove_reference<Args>::type &... args) const
		{
			std::vector<ReturnType> vec;
			for (const auto& obs : mObservers.at(event)) vec.push_back(obs(args...));
			return vec;
		};
    size_t observerCount(const Event& event) const
    {
      auto it = mObservers.find(event);
      if (it == mObservers.end()) return 0;
      else return it->second.size();
    };

    size_t observerCount(Event&& event) const
    {
      auto it = mObservers.find(std::move(event));
      if (it == mObservers.end()) return 0;
      else return it->second.size();
    };
  private:
		std::map<Event, std::vector<Observer>> mObservers;
	};

} // namespace muk
} // namespace gris