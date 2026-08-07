# Code Smell Report

**Project:** SE102.N21 (NESContra)
**Scope:** `src/` folder
**Stats:** 268 source files (80 headers, 184 .cpp); 132 `*State*` .cpp files; 109 `dynamic_cast` usages; 47 `#pragma region` markers; 24 `isFake`/`isEnemy` flag usages; 60+ `NULL`-vs-`nullptr`.
**Largest files:** GraphicsHelper.cpp (1229 lines), Bill.cpp (740), Bill.h (542), Stage2.cpp (523), Stage.cpp (437).

## Summary

The codebase is a C++ DirectX 11 Contra remake. It is readable and the game loop is solid, but it repeats the same class/state boilerplate across dozens of objects, contains a few God objects, ships a large block of dead commented code, and is littered with magic numbers.

## 1. God Object / Blob

- `src/Abstractions/Graphics/GraphicsHelper.cpp` (1229 lines) owns device init, shader compilation/loading, texture decoding, sprite batching, viewport updates, present/occlusion handling and a `Report()` helper — all static members plus ~20 anonymous-namespace globals. It violates single responsibility.
- `src/Objects/Bill/Bill.h` (542 lines) declares the `Bill` class plus the `BillState` base and ~22 concrete state classes in one header. `Bill` multiple-inherits 6 classes (`Entity`, `HasTextures`, `HasSprites`, `HasAnimations`, `CollidableEntity`, `HasWeapons`).
- `src/Objects/Scene/Scene.cpp` mixes scene-state machine plumbing with huge hand-written sprite/texture/animation tables (e.g. 52 `ENDING_FRAME_NN` entries, one per line).

## 2. Duplicate / Speculative State Classes

- 132 `*State*.cpp` files implement the same 5-method stub skeleton (`Exit`/`Enter`/`Render`/`Update`/`HandleInput`).
- Many return `NULL` immediately as a no-op. Example — `BillRunState::Update`, `BillNormalState::Update`, `BulletRState::Update`, `BulletSState::Update` (43 `return NULL;` in `BillState/*.cpp` alone).
- Near-identical pairs: `BulletRState`, `BulletSState`, `BulletLState`, `BulletMState` — same `Update` body, same empty `Exit`/`HandleInput`, differing only by the sound name and the `BULLET_ANIMATION_ID` enum passed to `Render`.

## 3. Dead / Commented-Out Code

- `src/Objects/Bill/BillState/BillNormalState.cpp` lines ~80–105: a ~26-line commented "FOR DEBUG PURPOSE" block (`DIK_W/S/R/G/H` hacks, a `new BillDeadState()`).
- `src/Objects/Bill/Bill.cpp` lines 100–114: a commented block `//int i = 1;` and a `// RSHIFT` bullet-type cycler.
- `src/Objects/Bill/Bill.cpp` lines 17–21: commented `//bill.SetVX(0.0f)` lines left inside `Enter`.

## 4. Magic Numbers

- Velocity/acceleration literals in states: `-2.0f`, `+2.0f`, `+1.0f`, `+4.00f`, `-0.10f`, `0.05f` (`BillJumpState.cpp`).
- Graphics tuning: `16384` instance-ring capacity, `4096` max sprites, `1 << 22` byte cap (`GraphicsHelper.cpp`).
- Hard-coded offsets: `20.0f`, `3.0f` (`Stage.cpp:139,156-168`), `256x240`/`0,0` sprite rects repeated 52 times (`Scene.cpp`).

## 5. Data Clumps / Primitive Obsession

- `BulletFState::Enter` (`src/Objects/Bullet/BulletState/BulletFState.cpp:16-44`) and `BulletLState::Enter` repeat the same 3-branch `if (vx!=0 && vy!=0)` / `if (vx==0 && vy!=0)` / `if (vx!=0 && vy==0)` chain with overlapping magic values. A direction/angle lookup table would replace it.
- The `0,0,256,240` sprite rectangle is copy-pasted for every `ENDING_FRAME_NN` entry in `Scene.cpp:56-105`.

## 6. Switch Statement That Should Be Polymorphism

- `src/Stages/Stage.cpp:107` — `switch (enemy->enemyType)` to choose an explosion type/sound. Each `Enemy` subclass could provide its own death-explosion factory, removing this central switch and the `dynamic_cast<Enemy<Bill>*>` branch.

## 7. Singleton Anti-Pattern + Hidden Global State

- `Sound::getInstance()` called from 10 sites across `Stage.cpp`, `Stage1.cpp`, `Stage2.cpp` and every weapon/bullet state.
- `main.cpp` keeps module-global `scene`, `input`, `camera` pointers instead of scoping them inside `WinMain` or a class.
- `GraphicsHelper` exposes globals (`device`, `context`, `swapChain`) and free functions used across modules.

## 8. Violates Open/Closed (Divergent Change)

- `src/Common.h:63` comment: *"If u add an object, u must add id of object here."* Every new object type must be manually appended to three `std::variant` lists (`SPRITE_ID`, `TEXTURE_ID`, `ANIMATION_ID`) and re-registered in `Bill.h`'s forward-declaration block — touching core shared headers every time.

## 9. Feature Envy

- State classes constantly reach into their host via `GetVX/SetVX/GetVY/SetVY/SetX/SetY/SetAngle` (e.g. `BillRunState`, `BillJumpState`) instead of exposing intent-revealing operations on `Bill`/`Bullet`.

## 10. Likely Copy-Paste Bug

- `BulletRState::Enter` (`src/Objects/Bullet/BulletState/BulletRState.cpp:18`) plays `"shootM"` — the *M*-state sound — suggesting it was copied from `BulletMState` and not renamed. Should be verified against intended behaviour.

## 11. Redundant Type Aliases

- `src/Common.h:142-143`:
  ```cpp
  using         TIME = DWORD;
  using DEFAULT_TIME = DWORD;
  ```
  Two names for the same type; one should be removed.

## 12. Overlapping Conditionals

- `BillJumpState::Update` (`src/Objects/Bill/BillState/BillJumpState.cpp:58-67`) uses two separate `if` blocks — `if (vy >= 0)` and `if (vy <= 0)` — which overlap at exactly `0` and both execute. An `else if` chain intended here.

## 13. Type Code via dynamic_cast (Replace Conditional with Polymorphism)

- `dynamic_cast` is used 109 times across the project as a substitute for virtual dispatch:
  - `Bill.cpp` (37), `Soldier.cpp` (20), `Bullet.cpp` (13), `Stage.cpp` (13), `HasWeapons.cpp` (10).
- `src/Objects/Bill/Bill.cpp` **lines 347-404** — `Bill::Fire` is itself a giant 11-branch `if (dynamic_cast<XState*>(state)) else if (dynamic_cast<YState*>(state))` that re-implements each state's bullet-spawn parameters (offset + velocity) **externally**, instead of asking the current state for them.
- `src/Stages/Stage.cpp:104-134` — `dynamic_cast<Enemy<Bill>*>` + `switch (enemy->enemyType)` for explosion selection (same smell, two flavors).
- `src/Abstractions/Object/Enemy.h` — `Enemy<T>` is a 52-line template mixin used only so it can be `dynamic_cast`'d to; it adds no behaviour beyond getters/setters.
- `src/Abstractions/Object/CollidableState.h:6` — a comment openly advertises the smell: *"DERIVING THIS CLASS OR JUST USE dynamic_cast<> AND THEN CHECK IF THE POINTER IS NULL"*.

## 14. Long Parameter Lists & Parameter Objects

- `HasWeapons::Fire(FLOAT x, FLOAT y, FLOAT angle, FLOAT vx, FLOAT vy, FLOAT ax, FLOAT ay, DIRECTION)` — 8 positional params, and the call sites pass the same 8-arg literal with only 1-2 values changed (`#9` already notes `Bill::Fire`).
- `GraphicsHelper::InsertSprite(SpriteID, x, y, w, h, dir, TextureID)` and `InsertAnimation(AnimID, time, {...})` repeated hundreds of times.

## 15. Duplicated Guard Logic

- `Bill::StaticResolveNoCollision`, `StaticResolveOnCollision` and `DynamicResolveNoCollision` (lines 406-456) each begin with the identical copy-pasted block:
  ```cpp
  if (dynamic_cast<BillDeadState*>(state)) { surfaceEntity = NULL; isAbSurface = 0; return; }
  ```
- `Bullet::Render`/`Bill::Render` duplicate the same `if (updateState){ ChangeState(...); updateState = NULL; } if (handleInputState){...}` transition block.

## 16. Confused / Dead Ownership Code

- `Bullet::SetState` (line 67) ends with `newState = NULL;` — but `newState` is passed by value, so the null is a no-op leftover.
- `HasWeapons(BulletState*)` (HasWeapons.cpp:13) transfers ownership by writing `this->bulletState = bulletState; bulletState = NULL;` on the by-value parameter — another dead null that only looks like it did something.
- `QuadTreeNode::Insert`/`Clear` use raw `new`/`Destroy` with comma-operator cleanup (`if (...) this->nodes[i]->Clear(), Destroy(this->nodes[i]);`) instead of RAII.

## 17. Singleton Anti-Pattern (Sound)

- `src/Abstractions/Device/Sound.h/.cpp`: hand-rolled singleton (`static Sound* instance`, `create()`, `getInstance()`).
- `cleanUp()` does `delete this;` (`#17`) — a self-deleting singleton whose `getInstance()` pointer then dangles; `mute()` calls `getInstance()->stop()` afterward.
- Stores raw `IDirectSoundBuffer8*` in a `std::map<std::string,...>` and `Release()`s them manually in the destructor — no RAII / `ComPtr`.

## 18. NULL vs nullptr / C-Style Casts

- `NULL` is used for pointers across ~15 files (Sound.cpp 13, Soldier.cpp 12, Bill.cpp 12, …) instead of `nullptr`.
- C-style casts sprinkled in: `(char*)sfileName` (Sound.cpp:81), `(HANDLE)` (main.cpp:64), `(FLOAT)`, `(INT)` in several Entity headers.

## 19. Include-Everything / Coupling

- `Bill.cpp` pulls in 14 headers (`Bill.h`, `Bullet.h`, `Bridge.h`, `Falcon.h`, `Soldier.h`, …); `Bullet.cpp` pulls in 12 including `Bill.h` — so Bill↔Bullet mutually include each other to resolve state types. Many translation units include a dozen "Common.h"-style aggregate headers.
- 47 `#pragma region`/`#pragma endregion` markers exist purely to fold over-sized `LoadSprites`/`LoadTextures`/`LoadAnimations` tables.

## 20. Near-Identical Physics Helpers (Motion)

- `Motion::CalculateUniformlyAcceleratedMotion` and `CalculateUniformlyDeceleratedMotion` are byte-for-byte identical except the sign-flip test (`v*a <= 0` vs `v*a >= 0`) — duplicated formula.
- Uses `pow(t, 2.0f)` and even `pow(ω, 1.0f)` (the latter is a no-op) where plain multiplication is clearer.

## 21. Minor: Data Duplication & Non-Portable Identifiers

- `BILL_SPRITE_ID::BEGIN_01..04` reuse the **exact** sprite rectangles as `JUMP_01..04` (`Bill.cpp:131-134` vs `146-149`) — almost certainly an accidental copy.
- `BulletFState` / `BulletScubaSoldierState` use Unicode identifiers (`ω`, `θ`, `dω`, `xO`, `yO`) (`#9`/`Bullet.h:182-186`), hurting grep-ability and portability.

## 22. Incomplete / Experimental Code

- `src/Abstractions/Space/QuadTreeNode.cpp:59` — `QuadTreeNode::Remove` is left with the comment *"THIS METHOD IS STILL UNDER EXPERIMENTS"* and appears unfinished.
- `QuadTreeNode::Clear`/`Clean` duplicate the child-walk loop and use magic index `i <= 3`.

## 23. A Third Type-Code Chain (Bullet Collision) + Boolean-Flag-as-Type

- `Bullet::DynamicResolveOnCollision` (`src/Objects/Bullet/Bullet.cpp:188-288`) is a third large type-code chain: 11 sequential `if (dynamic_cast<X*>(aabbSweepResult.surfaceEntity))` blocks (RockFly, Bridge, `Enemy<Bill>`, Fire, GunBossStage1, FinalBossStage1, Bill, plus state-driven checks for `BulletScubaSoldierState`/`BulletBossStage1State` and `TerrainBlock`).
- Two of those blocks (ScubaSoldier vs BossStage1) are near-identical, differing only in the `TERRAIN_BLOCK_TYPE` case list — duplicated collision rules.
- Boolean flags `isFake`/`isEnemy` (24 usages) on `Bullet`/states choose enemy-vs-player behaviour instead of subclassing (the `isEnemy` flag toggles collision responses rather than using distinct `BulletEnemyState` vs player-state behaviour).
- `Bullet.h` (299 lines) declares `Bullet` + 13 state classes in one header — the same dump-header pattern as `Bill.h` (§1).

## 24. Suspicious Abstractions & Naming

- `src/Common.h:60` — `template<class T> struct Bool { BOOL value; };`: a struct wrapping a single `BOOL` only so each `HasSprites<T>`/`HasTextures<T>`/`HasAnimations<T>` has a distinct static lazy-init flag. Every `LoadSprites/Textures/Animations` reads `hasBeenLoaded.value`. Over-engineered, and the flag is never reset (once loaded, always loaded for the process).
- `GraphicsHelper::InsertTexure` (missing `r`) — misspelled API, used across `Scene.cpp`, `Bullet.cpp`, `Bill.cpp` (§5/§15 data tables) and declared in `GraphicsHelper.h:64`.
- `Sound::play(std::string name, bool infiniteLoop, int times)` mixes loop semantics and uses an uncommented `Play(0, 0, times - 1)` (DirectSound loop count is 0-based); `loadSound` takes `const char*` file + `std::string name` — inconsistent string types vs the rest of the project's `LPCWSTR`/`std::wstring`.

## 25. Shared Mutable State / Global Collections

- `HasWeapons::bullets` is a `static std::list<Bullet*>` shared by every weapon; `Stage::Update` drains and `clear()`s it globally (`Stage.cpp:61-67`) — hidden coupling between weapons, stage and scene.
- `Sound` singleton owns a global `soundBufferMap`; `main.cpp` keeps module-global `scene`/`input`/`camera`.

## 26. Boolean-Flag State Machine (BossStage3Joint) + Divergent Large Class

- `BossStage3Joint::Update()` (`src/Objects/Enemies/BossStage3/BossStage3Hand/BossStage3Joint.cpp:46-72`) dispatches over **13 boolean members** (`isMoveAround`, `isMoveTo`, `isMoveBy`, `isMoveByVelocity`, `isMoveFollow`, `allowMoveReverse`, `isMoveAroundAcclemetor`, `isJointFollowStoped`, `isMoveFollowFirstTime`, `isMoveFollowWithAccelemetor`, …) each set independently and each checked with a separate `if` (not `if/else`), so several movement modes can run in one frame and rely on an implicit ordering. A single state enum / proper state pattern would replace 13 flags.
- Magic numbers litter the movement math: `16` (radius/align distance), `2.5f`, `0.7f`, `11.0f`, `2.0f * 3.1415926f` (`#26`).
- Misspelled identifiers: `moveAroundAccelemetor`, `isMoveAroundAcclemetor` (missing `r`), `MoveAroundDirection::Unknow` (missing `n`).
- Mixed-language comments: Vietnamese in English code (`//xoay xung quanh 1 diem`, `//doi huong di chuyen`).
- `LoadSprites`/`LoadTextures`/`LoadAnimations` are empty stubs deferring to the parent hand (`//textures had loaded in hand parent`) — a fragile shared-loading coupling.
- `HandleInput` is a commented-out leftover (line 83) from when the class dropped the state pattern.

## 27. Inconsistent Resource Management (GraphicsHelper)

- `GraphicsHelper::Init` uses `ComPtr` for some pipeline objects (`g_spriteVS`, `g_blendState`, etc.) but exposes `device`/`context`/`swapChain` as raw static pointers and `Release()`s them manually in `Cleanup()` (`GraphicsHelper.cpp:630-693`), where `ComPtr` would have handled teardown.
- `GraphicsDatabase::textures` stores raw `ID3D11ShaderResourceView*` that is manually `Release()`d+deleted in `Cleanup()` — no RAII, and the `RECT*` sprites are `new`'d/`delete`'d by hand.

## 28. Cross-Cutting Duplication & Dispatch Smells

- **Duplicated dead-state guard:** the block `if (dynamic_cast<XxxDieState*>(state)) { surfaceEntity = NULL; isAbSurface = 0; return; }` is copy-pasted verbatim across `Bill` and `Soldier` (and presumably others) in `StaticResolveNoCollision`/`StaticResolveOnCollision`/`DynamicResolveNoCollision`. It should be one shared `IsDying()` check, not repeated per class per method.
- **`Soldier::Fire` (line 32)** picks a fire-offset with a nested `dynamic_cast` ternary: `dynamic_cast<SoldierShootState*>(state) ? 0.75f : dynamic_cast<SoldierLayDownState*>(state) ? 0.20f : 0.00f;` — type code in expression form, plus magic coefficients.
- **`Stage2::TranslateWalls` (lines 63-81)** dispatches on wall **name strings** (`if (name == "L") else if (name == "R") else if (name == "B")`) — string-keyed `if`-chain where a map or the wall itself owning its alignment would do; and the `"B"` branch uses `wall->SetX(camera->GetX())` (centre) while L/R use `GetL()`/`GetR()` — an asymmetric, likely-incorrect position.
- **`Stage2::CheckIfHasDone` (line 55)** — magic completion threshold `bill->GetVY() <= -3.0f`.
- **Circular include:** `Bullet.cpp` includes `Bill.h` and `Bill.cpp` includes `Bullet.h` (and both include a dozen enemy headers) — mutual dependency resolved only by forward declarations in the headers.
- **`Sound::loadSound`** (lines 100-108) checks the WAVE format but does **not return** on mismatch; it prints an error and falls through to build a wave format from garbage header fields.

## 29. Reinvented Math Shim, God Template & Fragile Build

- **`DX11Math.h` (188 lines)** reinvents `D3DXVECTOR2/3`, `D3DXMATRIX`, color macros and vector helpers on top of DirectXMath "so game-logic files compile without modifications" — a compatibility shim that re-implements an existing library. It leans on `reinterpret_cast<const XMFLOAT4x4*>(&matrix)` between its own and DirectXMath's matrix types, betting on identical layout (a latent UB risk).
- **`HasAnimations::SetAnimation` (`HasAnimations.h:66-96`)** is a ~30-line god template inlined in a header: it indexes the animation, mutates `currentFrame`/`lastFrameTime`, reaches into `GraphicsDatabase::sprites`/`animations`, and calls `DrawSprite` — all as side effects, and stores `currentFrameW/H` as mutable cache read later by `Bill::Render`.
- **Dangerous `operator[]` on the variant-keyed maps** — `GraphicsDatabase::animations[animationId]` / `sprites[...]` (lines 70, 87) silently **default-insert** a bogus entry if an ID was never registered, instead of failing; hashing a ~20-way `std::variant` is also noted as non-trivial.
- **Global service-locator registry**: `GraphicsDatabase.h` is just three `static unordered_map` members keyed by `std::variant` — global mutable state no class owns.
- **`build.ps1`** hard-codes `C:\Program Files\Microsoft Visual Studio\2022\...` paths and filters MSBuild errors with `$_ -match "error"` (false-positives on "0 errors" / any line containing "error").

## Recommendation Priority (high → low)

1. Replace the 130+ `dynamic_cast` type-code chains with virtual dispatch (a `BulletSpawnParams()`/`Bullet*` factory on `BulletState`, an `OnDeathExplosion()` on `Enemy`, and collision handlers on `Entity`) — §13/§23 is the highest-leverage fix.
2. Delete the commented-out debug blocks (§3) and the experimental/unfinished `QuadTreeNode::Remove` (§22).
3. Collapse the 132 speculative state classes into a shared default base and remove the `return NULL;` stubs (§2, §11).
4. Replace boolean flags `isFake`/`isEnemy`/type-code `switch` with subclassing (§23).
5. Extract `GraphicsHelper.cpp` and the `LoadSprites/Textures/Animations` tables into data-driven loaders; fix the `InsertTexure` typo (§1, §19, §24).
6. Fix the `Sound` self-deleting singleton; replace raw COM pointers with `ComPtr` and C `fopen_s` with RAII/`<filesystem>` (§17, §24).
7. Remove the `Bool<T>` lazy-flag abstraction and the shared `HasWeapons::bullets` global; scope `scene/input/camera` inside `WinMain` (§24, §25, §7).
8. Remove duplicated guards/ownership no-ops (§15, §16) and replace `NULL`/`pow(x,1)` across the board (§18, §20).
9. Replace the 13 boolean flags in `BossStage3Joint` with a single movement-state enum (§26).
10. Make GraphicsHelper resource management uniformly RAII (`ComPtr`/helpers for `device`/`context`/`swapChain`/textures) (§27).
11. Fix identifier typos (`InsertTexure`, `Accelemetor`, `Unknow`) and delete Vietnamese/debug comments and empty defer-to-parent loaders (§24, §26).
12. Centralise the repeated dead-state guard and the `Bullet::Render`/`Bill::Render` transition block into shared helpers (§15, §28);
13. Drop the `DX11Math.h` shim in favour of `DirectX::XMFLOAT*` types and guard/`at()` map access to stop silent default-insertions (§29);
14. Hard-encode the VS install only as a fallback and match build errors with `error[0-9]` rather than the bare word `error` (§29).
