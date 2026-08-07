# Dead code in the source tree

> **Verified:** 2026-08-07, against the tree as it stands after the folder
> restructure. Method: grep for every symbol across `src/`, cross-checked against
> the set of files the four build configurations actually compile, plus a
> syntax-only `cl /Zs` compile of the files in question.

**14 files in `src/` are dead.** They are kept on purpose rather than deleted, so
this note exists to stop the next person assuming they are live code — three of
them are even listed in `NESContra.vcxproj`, which is exactly what makes them
look alive.

Nothing here affects the build: none of the 14 is compiled, and removing all of
them would not change a single byte of `NESContra.exe`.

## Summary

| Files | State | Compiles? |
|---|---|---|
| `Objects/Enemies/AirCraft/AirCraftState/*.cpp` (8) | abandoned state machine | **No** |
| `Objects/Fire/FireState/FireNormalState.cpp` | empty placeholder (0 bytes) | n/a |
| `Abstractions/Space/QuadTree.cpp`, `QuadTree.h`, `QuadTreeCommon.h`, `QuadTreeContainer.cpp`, `QuadTreeContainer.h` (5) | superseded quadtree design | untested |

---

## 1. The AirCraft state machine (8 files, cannot compile)

```
src/Objects/Enemies/AirCraft/AirCraftState/
    AirCraftNormalState.cpp      68 lines
    AirCraftInvulState.cpp       44 lines
    AirCraftBAmmoState.cpp       44 lines
    AirCraftFAmmoState.cpp       44 lines
    AirCraftLAmmoState.cpp       44 lines
    AirCraftMAmmoState.cpp       44 lines
    AirCraftRAmmoState.cpp       44 lines
    AirCraftSAmmoState.cpp       44 lines
```

Every one of these derives from `AirCraftState`:

```cpp
AirCraftNormalState::AirCraftNormalState() : AirCraftState()
```

**`AirCraftState` is declared nowhere in the project.** `AirCraft.h` declares only
`class AirCraft`, and there is no `class AirCraftState` in any header. So these
files cannot be added back to the build as they are — compiling one gives:

```
AirCraftNormalState.cpp(3): error C2653: 'AirCraftNormalState' is not a
                                        class or namespace name
  (30 errors total)
```

while `AirCraft.cpp`, sitting one folder up and *in* the build, compiles clean.

`AirCraft.cpp` has no state machine of any kind — the aircraft is driven directly.
The only reference to these classes from outside the folder is a commented-out
debug hotkey block in `Objects/Enemies/Falcon/FalconState/FalconOpeningState.cpp`
(around lines 46-73), which was presumably how the ammo states were being
tried out:

```cpp
	//invul, b, f, l, m, r, s
	/*if (input.IsKey(DIK_1))
	{
		return new AirCraftInvulState();
	}
	...*/
```

**Read as:** a state machine that was started, then abandoned, with its base class
removed afterwards. Reviving it means writing `AirCraftState` first and deciding
what an AirCraft's states are actually for.

## 2. `FireNormalState.cpp` — empty placeholder

```
src/Objects/Fire/FireState/FireNormalState.cpp    0 bytes
```

A zero-byte file. There is no `FireState` type anywhere in the project, and
`Fire.cpp` (which *is* in the build) has no state machine. The `FireState/`
folder exists only to hold this one empty file.

## 3. The superseded quadtree (5 files)

There are **two** quadtree implementations in `Abstractions/Space/`. Only one is used.

### Live — this is the quadtree the game runs on

```
QuadTreeNode.h      65 lines    in the build
QuadTreeNode.cpp   288 lines    in the build
```

Used by `Stages/Stage.h`, `Stages/Stage.cpp`, `Stages/Stage1.cpp`,
`Stages/Stage2.cpp`. Note that it does **not** include `QuadTree.h` — it is
self-contained and shares nothing with the files below.

### Dead — an earlier or parallel design, never wired up

```
QuadTree.h            124 lines    <- ClInclude in NESContra.vcxproj (misleading)
QuadTreeCommon.h       74 lines    <- ClInclude in NESContra.vcxproj (misleading)
QuadTreeContainer.h    90 lines    <- ClInclude in NESContra.vcxproj (misleading)
QuadTree.cpp            2 lines    just `#include "QuadTree.h"`, nothing else
QuadTreeContainer.cpp   0 bytes    empty
```

The reachability chain is closed:

- `QuadTree.h` is included by only two files — its own stub `QuadTree.cpp`, and
  `QuadTreeContainer.h`.
- `QuadTreeContainer.h` is included by nothing at all.
- `QuadTreeCommon.h` is included only by `QuadTree.h`, so it is reachable only
  through the dead chain. (`Common.h` pulls in every other `*Common.h` in the
  project — it does not mention this one.)

So all 288 lines of header here are unreachable from any compiled file. The three
`ClInclude` entries are what make them appear to be part of the project; a
`ClInclude` only puts a file in Solution Explorer, it never causes compilation,
so they cost nothing but they do mislead.

---

## If you decide to remove all this later

1. Delete the 14 files listed above.
2. Remove the three `ClInclude` lines for `QuadTree.h`, `QuadTreeCommon.h` and
   `QuadTreeContainer.h` from `NESContra.vcxproj`, and their entries in
   `NESContra.vcxproj.filters`.
3. Delete the commented-out block in `FalconOpeningState.cpp` (lines ~46-73) —
   it is the last thing naming the AirCraft states.
4. Drop `src\Abstractions\Space` from `GameIncludeDirs` only if you also removed
   `QuadTreeNode.h`, which you should not — it is live.
5. Rebuild all four configurations to confirm nothing was load-bearing:
   `.\build.ps1 -Config Debug` / `-Config Release`, each for `x64` and `x86`.

`Objects/Fire/FireState/` and `Objects/Enemies/AirCraft/AirCraftState/` would
then be empty and can go too.
