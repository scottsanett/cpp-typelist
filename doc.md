# Typelist

## Why a typelist?

```cpp
class WidgetFactory {
	public:
	virtual Window* CreateWindow() = 0;
	virtual Button* CreateButton() = 0;
	virtual ScrollBar* CreateScrollBar() = 0;
};
```

For this factory to work, one needs to make it possible to the user to create factories of __arbitrary collections of types__.

## Typelist wishlist

One needs to be able to:

1. create a `WidgetFactory` by passing a parameter list to an `AbstractFactory` template****:
	
	```cpp
	typedef AbstractFactory<Window, Button, ScrollBar> WidgetFactory;
	// can be replaced by `using` statements
	```
	This is impossible as templates cannot have a variable number of parameters

2. a template-like invocation for various `CreateXxx` functions, such as `Create<Window>()`, etc:
	```cpp
	template <class T>
		T* MakeRedWidget<WidgetFactory& factory> {
		T* pW = factory.Create<T>();
		pw->SetColor(RED);
		return pW;
	}
	```
	`Create<Xxx>` is not legal because virtual functions cannot be templates.


## Basic Algorithm for `IndexOf`

Input: Typelist `TLIST`, type `T`

Output: compile-time constexpr

if `TList` is `NullType`, the value is `-1`
else 
- if the Head of `TList` is `T`, the value is `0`
- else, compute the result of `IndexOf` to `TList`'s tail and T into `temp`
	* if `temp` is `-1`, then the value is `-1`
	* else `value` is `temp + 1`


## Appending to Typelists

Input: Typelist `TList`, type or typelist `T`

Output: Inner type definition `result`

if `TList` is `NullType` and `T` is `NullType`, `result` is `NullType`
else
- if `TList` is `NullType` and `T` is a single type, `result` is a typelist having `T` as the only element
- else
	- if `TList` is `NullType` and `T` is a typelist, result is `T`
	- else if `TList` is non-null, `result` is a typelist having `TList::Head` as its head, and the result of appending `T` to `TList::Tail as its tail.

## Erase a Type from Typelists

### Remove only the first occurrence

if `TList` is `NullType`:
- `Result` is `NullType`

else if `T` is `TList::Head`:
- `Result` is `TList::Tail`

else:
- Result is `Typelist` having `TList::Head` as its head and the result of applying `Erase` to `TList::Tail` and `T` as its tail


## Erase Duplicates

if `TList` is `NullType`, `result` is `NullType`

else
- apply `EraseDuplicates` to `TList::Tail` and yield a temporary `L1` (`L1` has no duplicates)
- apple `Erase` to `L1` and `TList::Head` and yield a temporary `L2` (remove all `TList::Head` occurrences in `L1`)
- Result is a typelist that has `TList::Head` as head and `L2` as tail.