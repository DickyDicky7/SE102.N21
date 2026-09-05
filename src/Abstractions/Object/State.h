#pragma once

#include "Input.h"
#include "Common.h"
#include "Entity.h"

template <class T, class E> requires std::derived_from<E, Entity>
class State
{

public:

	State();
	virtual ~State();

	virtual void Exit(E& entity) = 0;
	virtual void Enter(E& entity) = 0;
	virtual void Render(E& entity) = 0;

	virtual T* Update(E& entity) = 0;
	virtual T* HandleInput(E& entity, Input& input) = 0;

};

template <class T, class E> requires std::derived_from<E, Entity>
inline State<T, E>::State()
{
}

template <class T, class E> requires std::derived_from<E, Entity>
inline State<T, E>::~State()
{
}

// ---------------------------------------------------------------------------
// Deferred state transitions
//
// Update and HandleInput do not swap the state in themselves: they hand one back
// and it is queued with DeferState, then applied with ApplyDeferredState at the
// END of the owner's Update - the last thing a logic step does for that entity.
//
// Applying it there, rather than from Render where this used to live, is what
// makes the collision pass authoritative.  One logic step runs
//
//     HandleInput  ->  Update  ->  Stage::CheckResolveClearCollision  ->  (Render)
//
// and the collision handlers - Bill::DynamicResolveOnCollision and friends -
// call ChangeState directly and immediately.  While transitions were applied
// from Render, they ran AFTER that pass and overwrote its decision with one
// computed before the collision was known: a landing, a drowning or a lethal
// touch resolved during collision could be undone in the same frame, e.g. a
// jump whose Update returned BillNormalState would cancel the BillDeadState an
// enemy contact had just installed.  Draining the queue at the end of Update
// leaves nothing pending by the time collision runs, so its ChangeState is the
// last word - and Render goes back to being purely presentational, which also
// means an entity that is updated without being drawn no longer freezes.
// ---------------------------------------------------------------------------

// Records a state transition to be applied at the end of this Update, replacing
// any earlier transition that has not been applied yet.
//
// The fixed-timestep loop in WinMain runs up to five logic steps per rendered
// frame when it has time to catch up on, so before transitions moved out of
// Render a plain `pending = _state->Update(...)` could overwrite - and leak - the
// state a previous step had asked for.  Keeping the newest request and freeing
// the one it supersedes stays correct now that Update itself drains the queue.
//
// A null `next` means "no transition this step" and leaves any pending one
// alone.
template <class S>
inline void DeferState(S*& pending, S* next)
{
	if (!next || next == pending)
		return;

	delete pending;
	pending = next;
}

template <class BS, class DS, class E> requires std::derived_from<DS, BS> && std::derived_from<BS, State<BS, E>> && std::derived_from<E, Entity>
inline void ChangeState(BS*& currentState, DS* newState, E* contextEntity)
{
	if (currentState == newState)
		return;

	if (currentState)
	{
		currentState->Exit(*contextEntity);
		delete currentState;
	}
	currentState = newState;
	if (currentState)
	{
		currentState->Enter(*contextEntity);
	}
}

// Applies a transition queued by DeferState, if there is one.  Call at the end
// of Update - see the note above for why it must not be called from Render.
//
// Where a step queued one from HandleInput and one from Update, call this for
// the update queue first and the handle-input queue second: that is the order
// Render applied them in, so a step that queued both still ends on the input's
// choice with the update's Enter having run.
template <class S, class E>
inline void ApplyDeferredState(S*& currentState, S*& pending, E* contextEntity)
{
	if (!pending)
		return;

	// Cleared before Enter runs, so that a transition Enter itself queues is not
	// swallowed by this one on the way out.
	S* next = pending;
	pending = nullptr;

	ChangeState(currentState, next, contextEntity);
}

